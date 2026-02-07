#pragma once

namespace SDS
{

	template <class T>
	T* GetTESFormFromSetting(std::string &confLine)
	{
		return RE::TESDataHandler::GetSingleton()->LookupForm<T>(std::stoi(confLine, nullptr, 16), confLine.substr(9));
	}

	enum class SDSFocus : int
	{
		kCombat = 1,
		kMagick = 2,
		kStealth = 3,
		kNone = 4
	};

	const char* SDSFocusToString(SDSFocus a_focus);
	SDSFocus SDSFocusByName(const char* a_name);
	SDSFocus SDSFocusByAV(RE::ActorValue av);

	enum SDSAttribute : int
	{
		kStrength = 1,
		kIntelligence = 2,
		kWillpower = 3,
		kAgility = 4,
		kSpeed = 5,
		kEndurance = 6,
		kPersonality = 7,
		kLuck = 8,
		kFaith = 9,
		kNone = 10
	};

	struct SDSLeveledValue
	{
		RE::ActorValue av;
		std::map<int, float> leveledValues;
		const char* name;
		uint16_t perkEntryIndex;

		SDSLeveledValue(const RE::ActorValue& a_av, const char* a_name) :
			av(a_av), name(a_name), perkEntryIndex(0)
		{
		}

		float GetClosest(int playerLevel);
	};

	struct SDSLeveledAttribute
	{
		// Core attribute this struct are represent
		SDSAttribute attribute;
		// List of actor values this struct are affect
		std::vector<SDSLeveledValue> actorValues;
		// String representation of attribute
		const char* name;
		// Perk entry if attribute have perk dependant on it
		// TODO may be add list of perks
		RE::BGSPerk* perkEntry;
		// Additional starter racial bonuses
		std::map<RE::FormID, int> racialBonuses;

		SDSLeveledAttribute(const SDSAttribute& attr) :
			attribute(attr)
		{
			name = SDSLeveledAttribute::NameByAttribute(attr);
			racialBonuses = {
				// Argonian, Breton, Dark Elf, High Elf, Imperial
				{ 0x13740, 0 }, { 0x13741, 0 }, { 0x13742, 0 }, { 0x13743, 0 }, { 0x13744, 0 },
				// Khajiit, Nord, Orc, Redguard, Wood Elf
				{ 0x13745, 0 }, { 0x13746, 0 }, { 0x13747, 0 }, { 0x13748, 0 }, { 0x13749, 0 }
			};
		}

		void ToGFxValue(RE::Character* actor, int multiplier, int playerLevel, RE::GFxValue* value);
		float GetAttributePerkFunctionValue(RE::Character*, SDSLeveledValue* val);

		RE::ActorValue GetAccociatedActorValue();

		float GetBaseActorValue(RE::Character*);
		void SetBaseActorValue(RE::Character*, float);
		void IncrementAttribute(RE::Character*, float val);

		static const char* NameByAttribute(SDSAttribute attr);
	};

	SDSAttribute SDSAttributeByName(const char* attr_name);

	enum class InventoryDefine : int
	{
		kNone = 0,
		kArmor = 1,
		kWeapon = 2,
		kMisc = 3,
		kBook = 4,
		kFood = 5,
		kPotion = 6,
		kSpell = 7,
		kIngredient = 8,
		kKey = 9,
		kShout = 10,
		kActiveEffect = 11,
		kSoulGem = 12,
		kSpellDefault = 13,
		kList = 14,
		kCraftEnchanting = 15,
		kHousePart = 16
	};

	struct PlayerData
	{
		std::string SelectedSpecializationID;
		int RemainingSkillPoints;

		int CombatSkillPoints;
		int StealthSkillPoints;
		int MagicSkillPoints;

		PlayerData(const std::string& SelectedSpecializationID, int RemainingSkillPoints) :
			SelectedSpecializationID(SelectedSpecializationID), RemainingSkillPoints(RemainingSkillPoints)
		{
		}

		PlayerData(){
			SelectedSpecializationID = "";
			RemainingSkillPoints = 0;

			CombatSkillPoints = 0;
			StealthSkillPoints = 0;
			MagicSkillPoints = 0;
		};
	};
}
