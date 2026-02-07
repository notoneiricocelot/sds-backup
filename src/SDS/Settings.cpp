#include "Settings.h"
#include "Specialization.h"

#include <toml.hpp>

namespace SDS
{

	void ReadBoolSetting([[maybe_unused]] CSimpleIniA& a_ini, [[maybe_unused]] const char* a_sectionName, const char* a_settingName, bool& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
		}
	}

	void ReadUInt32Setting([[maybe_unused]] CSimpleIniA& a_ini, [[maybe_unused]] const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
		}
	}

	void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (a_settingName) {
			a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
		}
	}

	void ReadTomlConfig(std::string path)
	{
		SKSE::log::info("Reading {}", path);

		try
		{
			toml::value data = toml::parse(path);
			std::vector<toml::value> classes = toml::find<std::vector<toml::value>>(data, "class");

			// load available classes list
			int i = 0;
			for (auto& c : classes)
			{
				std::string ID = toml::find<toml::string>(c, "id");
				std::string description = toml::find<toml::string>(c, "description");
				SDSFocus focus = static_cast<SDSFocus>(toml::find<toml::integer>(c, "focus"));
				SDSAttribute a1 = static_cast<SDSAttribute>(toml::find<toml::integer>(c, "attr1"));
				SDSAttribute a2 = static_cast<SDSAttribute>(toml::find<toml::integer>(c, "attr2"));

				Specialization::Specializations.push_back(std::make_unique<Specialization>(ID, fmt::format("${}", ID), description, focus, a1, a2));
				Specialization* spec = Specialization::Specializations.back().get();
				spec->equipment = toml::find<toml::string>(c, "equipment");

				std::vector<int> skills = toml::find<std::vector<int>>(c, "skills");
				for (i = 0; i < skills.size(); i++)
				{
					spec->skills[i] = skills[i];
				}
			}

			classes.clear();

			toml::value generalSettings = toml::find<toml::value>(data, "generalSettings");
			Settings::bDebugEnabled = static_cast<bool>(toml::find<toml::integer>(generalSettings, "bDebugEnabled"));
			Settings::iLogLevel = static_cast<int>(toml::find<toml::integer>(generalSettings, "iLogLevel"));

			SKSE::log::info("{} classes loaded, processing leveling settings", Specialization::Specializations.size());

			// processing static leveling settings
			toml::value levelingSettings = toml::find<toml::value>(data, "levelingSettings");

			Settings::iMaxLevelsPerSkillPerPlayerLevel = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iMaxLevelsPerSkillPerPlayerLevel"));

			Settings::iSkillPointCost0 = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iSkillPointCost0"));
			Settings::iSkillPointCost25 = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iSkillPointCost25"));
			Settings::iSkillPointCost50 = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iSkillPointCost50"));
			Settings::iSkillPointCost75 = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iSkillPointCost75"));

			Settings::iSkillPointsPerLevel = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iSkillPointsPerLevel"));
			Settings::iAttributePointsPerLevel = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iAttributePointsPerLevel"));

			Settings::iSkillPointsCap = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iSkillPointsCap"));
			Settings::fSkillPointsLevelMultiplier = static_cast<float>(toml::find(levelingSettings, "fSkillPointsLevelMultiplier").as_floating(std::nothrow));

			Settings::iBaseAttributeValue = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iBaseAttributeValue"));
			Settings::iClassBaseAttributeBonus = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iClassBaseAttributeBonus"));
			Settings::iClassBaseSkillBonus = static_cast<int8_t>(toml::find<toml::integer>(levelingSettings, "iClassBaseSkillBonus"));

			Settings::iAttributeCap = static_cast<int>(toml::find<toml::integer>(levelingSettings, "iAttributeCap"));

			assert(Settings::iSkillPointsPerLevel > 0);
			SKSE::log::info("Leveling settings loaded, processing racial bonuses");

			// Load racial bonuses
			toml::table racialBonuses = toml::find<toml::table>(data, "racialBonuses");

			RE::FormID raceForm;
			std::array<int, 9> raceAttributes;

			for (const auto& raceData : racialBonuses)
			{
				raceForm = std::stoul(raceData.first.c_str(), nullptr, 16);
				raceAttributes = toml::get<std::array<int, 9>>(raceData.second);

				i = 0;
				for (; i < Settings::LevelingSettings.size(); i++)
				{
					Settings::LevelingSettings[i].racialBonuses[raceForm] = raceAttributes[i];
				}
			}

			SKSE::log::info("Racial bonuses updated, processing attribute leveling tables");

			// Load leveled attributes setting config
			int leveledKey = 0;
			float leveledValue = 0.0f;
			toml::table avConfigTable;

			for (auto it = Settings::LevelingSettings.begin(); it < Settings::LevelingSettings.end(); it++)
			{

				for (auto avIt = it->actorValues.begin(); avIt < it->actorValues.end(); avIt++)
				{
					if (Settings::bDebugEnabled)
						SKSE::log::info("Processing subtable: {}.{}", it->name, avIt->name);

					// parsing subtables for level depended attributes
					if (data.contains(it->name) && data.at(it->name).contains(avIt->name))
					{
						avConfigTable = toml::find<toml::table>(data, it->name, avIt->name);
						for (const auto& kv : avConfigTable)
						{
							leveledKey = std::atoi(kv.first.c_str());
							leveledValue = static_cast<float>(kv.second.as_floating(std::nothrow));
							avIt->leveledValues.insert({ leveledKey, leveledValue });
						}
					}
					else if (Settings::bDebugEnabled && avIt->av != RE::ActorValue::kNone)
						SKSE::log::info("Subtable: {}.{} not found", it->name, avIt->name);
				}
			}
			SKSE::log::info("Attribute tables loaded");
		}
		catch (const std::exception& e)
		{
			util::report_and_fail(e.what());
		}
		catch (...)
		{
			util::report_and_fail("Configuration file incomplete"sv);
		}

		SKSE::log::info("Reading config completed, loaded {} leveled settings for attributes", Settings::LevelingSettings.size());
	}

	void Settings::Initialize()
	{
		// Setup leveling settings struct
		Settings::LevelingSettings.emplace_back(SDSAttribute::kStrength);
		Settings::LevelingSettings[0].actorValues.emplace_back(RE::ActorValue::kNone, "TwohandedDamage");
		Settings::LevelingSettings[0].actorValues.emplace_back(RE::ActorValue::kResistFire, "FireResist");
		Settings::LevelingSettings[0].actorValues.emplace_back(RE::ActorValue::kResistShock, "ElectricResist");
		Settings::LevelingSettings[0].actorValues.emplace_back(RE::ActorValue::kCarryWeight, "CarryWeight");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kIntelligence);
		Settings::LevelingSettings[1].actorValues.emplace_back(RE::ActorValue::kNone, "SpellPower");
		Settings::LevelingSettings[1].actorValues.emplace_back(RE::ActorValue::kNone, "SpellDuration");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kWillpower);
		Settings::LevelingSettings[2].actorValues.emplace_back(RE::ActorValue::kMagicka, "Magicka");
		Settings::LevelingSettings[2].actorValues.emplace_back(RE::ActorValue::kResistMagic, "MagicResist");
		Settings::LevelingSettings[2].actorValues.emplace_back(RE::ActorValue::kMagickaRateMult, "MagickaRateMult");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kAgility);
		Settings::LevelingSettings[3].actorValues.emplace_back(RE::ActorValue::kNone, "OnehandedDamage");
		Settings::LevelingSettings[3].actorValues.emplace_back(RE::ActorValue::kPoisonResist, "PoisonResist");
		Settings::LevelingSettings[3].actorValues.emplace_back(RE::ActorValue::kCarryWeight, "CarryWeight");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kSpeed);
		Settings::LevelingSettings[4].actorValues.emplace_back(RE::ActorValue::kStamina, "Stamina");
		Settings::LevelingSettings[4].actorValues.emplace_back(RE::ActorValue::kNone, "MarksmanDamage");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kEndurance);
		Settings::LevelingSettings[5].actorValues.emplace_back(RE::ActorValue::kHealth, "Health");
		Settings::LevelingSettings[5].actorValues.emplace_back(RE::ActorValue::kResistFrost, "FrostResist");
		Settings::LevelingSettings[5].actorValues.emplace_back(RE::ActorValue::kResistDisease, "DiseaseResist");
		Settings::LevelingSettings[5].actorValues.emplace_back(RE::ActorValue::kCarryWeight, "CarryWeight");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kPersonality);
		Settings::LevelingSettings[6].actorValues.emplace_back(RE::ActorValue::kDamageResist, "DamageResist");
		Settings::LevelingSettings[6].actorValues.emplace_back(RE::ActorValue::kStaminaRateMult, "StaminaRateMult");

		Settings::LevelingSettings.emplace_back(SDSAttribute::kLuck);
		Settings::LevelingSettings[7].actorValues.emplace_back(RE::ActorValue::kNone, "Abundance");
		Settings::LevelingSettings.emplace_back(SDSAttribute::kFaith);
		Settings::LevelingSettings[8].actorValues.emplace_back(RE::ActorValue::kNone, "EquipmentPower");

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler)
		{
			// Strength perk
			Settings::LevelingSettings[0].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000800, "SDSInterface.esp");
			// Intellegence perk
			Settings::LevelingSettings[1].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000801, "SDSInterface.esp");
			// Willpower perk
			Settings::LevelingSettings[2].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000802, "SDSInterface.esp");
			// Agility perk
			Settings::LevelingSettings[3].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000803, "SDSInterface.esp");
			// Speed perk
			Settings::LevelingSettings[4].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000804, "SDSInterface.esp");
			// Endurance perk
			Settings::LevelingSettings[5].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000805, "SDSInterface.esp");
			// Personality perk
			Settings::LevelingSettings[6].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000806, "SDSInterface.esp");
			// Luck perk
			Settings::LevelingSettings[7].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000807, "SDSInterface.esp");
			// Faith perk
			Settings::LevelingSettings[8].perkEntry = dataHandler->LookupForm<RE::BGSPerk>(0x000808, "SDSInterface.esp");
		}
	}

	void Settings::ReadSettings()
	{
		const std::string dataPath("Data/SKSE/Plugins/SDS.toml");

		constexpr const wchar_t* experienceIniPath = L"Data/SKSE/Plugins/Experience.ini";
		constexpr const wchar_t* uncapperIniPath = L"Data/SKSE/Plugins/SkyrimUncapper.ini";

		// Experience INI Settings
		const auto readExperienceIni = [&](std::filesystem::path path)
		{
			CSimpleIniA experienceIni;
			experienceIni.SetUnicode();
			experienceIni.LoadFile(path.string().c_str());

			ReadFloatSetting(experienceIni, "General", "fSkillCapBase", fSkillCapBase_Experience);
			ReadFloatSetting(experienceIni, "General", "fSkillCapMult", fSkillCapMult_Experience);

			ReadUInt32Setting(experienceIni, "General", "iMaxPlayerLevel", (uint32_t&)iMaxPlayerLevel_Experience);

			ReadBoolSetting(experienceIni, "General", "bUseRacialCaps", bUseRacialCaps_Experience);
		};

		const auto readUncapperIni = [&](std::filesystem::path path)
		{
			CSimpleIniA uncapperIni;
			uncapperIni.SetUnicode();
			uncapperIni.LoadFile(path.string().c_str());

			ReadUInt32Setting(uncapperIni, "SkillCaps", "iAlchemy", (uint32_t&)iMaxSkillLevel_Uncapper);
		};

		ReadTomlConfig(dataPath);
		readExperienceIni(experienceIniPath);
		readUncapperIni(uncapperIniPath);
	}

	void Settings::OnPostLoadGame()
	{
		Settings::selectedSpecIndex = Specialization::Find(Settings::Saved->SelectedSpecializationID);

		RE::ActorValueList* playerAVs = RE::ActorValueList::GetSingleton();

		// disable skill experience gain from skills use
		for (RE::ActorValue i = RE::ActorValue::kOneHanded; i <= RE::ActorValue::kEnchanting;)
		{
			playerAVs->GetActorValue(i)->skill->useMult = 0;

			i = static_cast<RE::ActorValue>(static_cast<int>(i) + 1);
		}
	}

	void Settings::OnRevert([[maybe_unused]] SKSE::SerializationInterface* serde)
	{
		std::unique_lock lock(_lock);

		Settings::Saved->SelectedSpecializationID = "";
		Settings::Saved->RemainingSkillPoints = 0;
		Settings::Saved->CombatSkillPoints = 0;
		Settings::Saved->StealthSkillPoints = 0;
		Settings::Saved->MagicSkillPoints = 0;
	}

	void Settings::OnGameLoaded(SKSE::SerializationInterface* serde)
	{
		std::unique_lock lock(_lock);

		std::uint32_t type = 0;
		std::uint32_t size = 0;
		std::uint32_t version = 0;

		while (serde->GetNextRecordInfo(type, version, size))
		{
			if (type == SpecializationRecord)
			{
				serde->ReadRecordData(&Settings::Saved->SelectedSpecializationID, sizeof(Settings::Saved->SelectedSpecializationID));
				serde->ReadRecordData(&Settings::Saved->RemainingSkillPoints, sizeof(Settings::Saved->RemainingSkillPoints));

				serde->ReadRecordData(&Settings::Saved->CombatSkillPoints, sizeof(Settings::Saved->CombatSkillPoints));
				serde->ReadRecordData(&Settings::Saved->StealthSkillPoints, sizeof(Settings::Saved->StealthSkillPoints));
				serde->ReadRecordData(&Settings::Saved->MagicSkillPoints, sizeof(Settings::Saved->MagicSkillPoints));
			}
		}
		SKSE::log::info("Config loaded, class: {}, remaining skillPoints: {}", Settings::Saved->SelectedSpecializationID, Settings::Saved->RemainingSkillPoints);
	}

	void Settings::OnGameSaved(SKSE::SerializationInterface* serde)
	{
		std::unique_lock lock(_lock);

		if (!serde->OpenRecord(SpecializationRecord, 0))
		{
			SKSE::log::error("Unable to open record to write cosave data.");
			return;
		}

		serde->WriteRecordData(&Settings::Saved->SelectedSpecializationID, sizeof(Settings::Saved->SelectedSpecializationID));
		serde->WriteRecordData(&Settings::Saved->RemainingSkillPoints, sizeof(Settings::Saved->RemainingSkillPoints));

		serde->WriteRecordData(&Settings::Saved->CombatSkillPoints, sizeof(Settings::Saved->CombatSkillPoints));
		serde->WriteRecordData(&Settings::Saved->StealthSkillPoints, sizeof(Settings::Saved->StealthSkillPoints));
		serde->WriteRecordData(&Settings::Saved->MagicSkillPoints, sizeof(Settings::Saved->MagicSkillPoints));
	}

}
