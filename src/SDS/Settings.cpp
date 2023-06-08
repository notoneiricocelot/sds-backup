#include "Settings.h"
#include "Specialization.h"

#include <toml.hpp>

namespace SDS
{

	void InitializeHook()
	{
		// The trampoline can be used to write a new call instruction at a given address (here the start of the function for
		// HitData::Populate). We use write_code<5> to indicate this is a 5-byte call instruction (rather than the much
		// rarer 6-byte call). We pass in the address of our function that will be called, and a pointer to the trampoline
		// function is returned.
		//
		// The trampoline pointed to contains any instructions from the original function we overwrote and a call to the
		// instruction that comes after, so that if we call that address as a function, we are in effect calling the
		// original code.
		// OriginalPopulateHitData = trampoline.write_call<5>(GetHookedFunction().address(),
		//												reinterpret_cast<uintptr_t>(PopulateHitData));
		SKSE::log::debug("Hooks initialized.");
	}

	void ReadTomlConfig(std::string path)
	{
		SKSE::log::info("Reading {}", path);

		try {
			const toml::value data = toml::parse(path);
			const std::vector<toml::value> classes = toml::find<std::vector<toml::value>>(data, "class");

			for (auto& c : classes) {
				int i = 0;
				std::string ID = toml::find<toml::string>(c, "id");
				std::string description = toml::find<toml::string>(c, "description");
				SDSFocus focus = static_cast<SDSFocus>(toml::find<toml::integer>(c, "focus"));
				SDSAttribute a1 = static_cast<SDSAttribute>(toml::find<toml::integer>(c, "attr1"));
				SDSAttribute a2 = static_cast<SDSAttribute>(toml::find<toml::integer>(c, "attr2"));

				Specialization::Specializations.push_back(std::make_unique<Specialization>(ID, fmt::format("${}", ID), description, focus, a1, a2));
				Specialization* spec = Specialization::Specializations.back().get();

				std::vector<int> skills = toml::find<std::vector<int>>(c, "skills");
				for (i = 0; i < skills.size(); i++) {
					spec->SetSkillFocus(skills[i], i);
				}
			}
		} catch (const std::exception& e) {
			util::report_and_fail(e.what());
		} catch (...) {
			util::report_and_fail("Configuration file incomplete"sv);
		}

		SKSE::log::info("Reading {} completed, loaded {} classes", path, Specialization::Specializations.size());
	}

	void Settings::Initialize()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			// esp variables lookup
		}
	}

	void Settings::ReadSettings()
	{
		const std::string dataPath("Data/SKSE/Plugins/SDS.toml");

		constexpr auto defaultSettingsPath = L"Data/MCM/Config/SDSInterface/settings.ini";
		constexpr auto mcmPath = L"Data/MCM/Settings/SDSInterface.ini";

		constexpr auto experienceIniPath = L"Data/SKSE/Plugins/Experience.ini";

		// Load class data and mod settings from toml file

		const auto readMCM = [&](std::filesystem::path path) {
			CSimpleIniA mcm;
			mcm.SetUnicode();
			mcm.LoadFile(path.string().c_str());

			ReadUInt32Setting(mcm, "General", "iMaxLevelsPerSkillPerPlayerLevel", (uint32_t&)iMaxLevelsPerSkillPerPlayerLevel);
			ReadUInt32Setting(mcm, "General", "iSkillPointCost0", (uint32_t&)iSkillPointCost0);
			ReadUInt32Setting(mcm, "General", "iSkillPointCost25", (uint32_t&)iSkillPointCost25);
			ReadUInt32Setting(mcm, "General", "iSkillPointCost50", (uint32_t&)iSkillPointCost50);
			ReadUInt32Setting(mcm, "General", "iSkillPointCost75", (uint32_t&)iSkillPointCost75);
			ReadUInt32Setting(mcm, "General", "iSkillPointsPerLevel", (uint32_t&)iSkillPointsPerLevel);
			ReadUInt32Setting(mcm, "General", "iSkillPointsCap", (uint32_t&)iSkillPointsCap);

			ReadBoolSetting(mcm, "General", "bSSLDebugEnabled", bSSLDebugEnabled);

			ReadFloatSetting(mcm, "General", "fSkillPointsLevelMultiplier", fSkillPointsLevelMultiplier);
		};

		const auto readExperienceIni = [&](std::filesystem::path path) {
			CSimpleIniA experienceIni;
			experienceIni.SetUnicode();
			experienceIni.LoadFile(path.string().c_str());

			ReadFloatSetting(experienceIni, "General", "fSkillCapBase", fSkillCapBase);
			ReadFloatSetting(experienceIni, "General", "fSkillCapMult", fSkillCapMult);

			ReadUInt32Setting(experienceIni, "General", "iMaxPlayerLevel", (uint32_t&)iMaxPlayerLevel);

			ReadBoolSetting(experienceIni, "General", "bUseRacialCaps", bUseRacialCaps);
		};

		ReadTomlConfig(dataPath);
		readMCM(defaultSettingsPath);
		readMCM(mcmPath);
		readExperienceIni(experienceIniPath);
	}

	void Settings::OnPostLoadGame()
	{

	}

	void Settings::ReadBoolSetting([[maybe_unused]] CSimpleIniA& a_ini, [[maybe_unused]] const char* a_sectionName, const char* a_settingName, bool& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
		}
	}

	void Settings::ReadUInt32Setting([[maybe_unused]] CSimpleIniA& a_ini, [[maybe_unused]] const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (bFound) {
			a_setting = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
		}
	}

	void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
	{
		const char* bFound = nullptr;
		bFound = a_ini.GetValue(a_sectionName, a_settingName);
		if (a_settingName) {
			a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
		}
	}

	void Settings::OnRevert([[maybe_unused]] SKSE::SerializationInterface* serde)
	{
		std::unique_lock lock(_lock);

		Settings::Saved->SelectedSpecializationID = "";
		Settings::Saved->RemainingSkillPoints = 0;
	}

	void Settings::OnGameLoaded(SKSE::SerializationInterface* serde)
	{
		std::unique_lock lock(_lock);

		std::uint32_t type = 0;
		std::uint32_t size = 0;
		std::uint32_t version = 0;
		// To load records from a cosave, use <code>GetNextRecordInfo</code> to iterate from one record to the next.
		// You will be given records in the order they were written, but otherwise you do not look up a record by its name.
		// Instead check the result of each iteration to find out which record it is and handle it appropriately.
		//
		// If you make breaking changes to your data format, you can increase the version number used when writing the data
		// out and check that number here to handle previous versions.
		while (serde->GetNextRecordInfo(type, version, size)) {
			if (type == SpecializationRecord) {
				serde->ReadRecordData(&Settings::Saved->SelectedSpecializationID, sizeof(Settings::Saved->SelectedSpecializationID));
				serde->ReadRecordData(&Settings::Saved->RemainingSkillPoints, sizeof(Settings::Saved->RemainingSkillPoints));
			}
		}
	}

	void Settings::OnGameSaved(SKSE::SerializationInterface* serde)
	{
		std::unique_lock lock(_lock);

		// To write data open a record with a given name. The name must be unique within your mod, and has a version number
		// assigned (in this case 0). You can increase the version number if making breaking format change from a previous
		// version of your mod, so that the load handler can know which version of the format is being used.
		if (!serde->OpenRecord(SpecializationRecord, 0)) {
			SKSE::log::error("Unable to open record to write cosave data.");
			return;
		}

		serde->WriteRecordData(&Settings::Saved->SelectedSpecializationID, sizeof(Settings::Saved->SelectedSpecializationID));
		serde->WriteRecordData(&Settings::Saved->RemainingSkillPoints, sizeof(Settings::Saved->RemainingSkillPoints));
	}

}
