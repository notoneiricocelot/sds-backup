#include "Common.h"

namespace SDS
{

	const char* SDSFocusToString(SDSFocus a_focus)
	{
		switch (a_focus) {
		case SDS::SDSFocus::kCombat:
			return "Combat";
		case SDS::SDSFocus::kMagick:
			return "Magick";
		default:
			return "Stealth";
		}
	}

	SDSFocus SDSFocusByName(const char* a_name)
	{
		SDSFocus attr = SDSFocus::kCombat;
		for (; attr <= SDSFocus::kStealth;)
		{
			if (strcmp(a_name, SDSFocusToString(attr)) == 0)
				return attr;

			attr = static_cast<SDSFocus>(static_cast<int>(attr) + 1);
		}

		return SDSFocus::kNone;
	}

	float SDSLeveledValue::GetClosest(int playerLevel)
	{
		// if config is simple
		if (this->leveledValues.size() == 1)
			return this->leveledValues.begin()->second;

		auto lower = this->leveledValues.lower_bound(playerLevel);
		return lower->second;
	}

	SDSAttribute SDSAttributeByName(const char* attr_name)
	{
		SDSAttribute attr = SDSAttribute::kStrength;
		for (; attr <= SDSAttribute::kFaith;)
		{
			if (strcmp(attr_name, SDSLeveledAttribute::NameByAttribute(attr)) == 0)
				return attr;

			attr = static_cast<SDSAttribute>(static_cast<int>(attr) + 1);
		}
		return SDSAttribute::kNone;
	}

	float SDSLeveledAttribute::GetAttributePerkFunctionValue([[maybe_unused]] SDSLeveledValue* val)
	{
		// TODO get perk data multiplier
		float perkMultiplier = 0.01f;
		return (1 + (GetBaseActorValue() * perkMultiplier));
	}

	void SDSLeveledAttribute::ToGFxValue(int multiplier, int playerLevel, RE::GFxValue* value)
	{
		double newValue;
		RE::GFxValue avValue;

		for (auto it = this->actorValues.begin(); it < this->actorValues.end(); it++)
		{
			if (it->av == RE::ActorValue::kNone)
			{
				newValue = multiplier * GetAttributePerkFunctionValue(it._Ptr);
			}
			else
			{
				newValue = it->GetClosest(playerLevel) * multiplier;

				if (value->HasMember(it->name))
				{
					value->GetMember(it->name, &avValue);
					newValue += avValue.GetNumber();
				}

			}

			value->SetMember(it->name, RE::GFxValue(newValue));
		}
	}

	RE::ActorValue SDSLeveledAttribute::GetAccociatedActorValue()
	{
		switch (attribute) {
		case SDS::SDSAttribute::kStrength:
			return RE::ActorValue::kBlockSkillAdvance;
		case SDS::SDSAttribute::kIntelligence:
			return RE::ActorValue::kInfamy;
		case SDS::SDSAttribute::kWillpower:
			return RE::ActorValue::kAlterationSkillAdvance;
		case SDS::SDSAttribute::kAgility:
			return RE::ActorValue::kAlchemySkillAdvance;
		case SDS::SDSAttribute::kSpeed:
			return RE::ActorValue::kFavorPointsBonus;
		case SDS::SDSAttribute::kEndurance:
			return RE::ActorValue::kFavorsPerDayTimer;
		case SDS::SDSAttribute::kPersonality:
			return RE::ActorValue::kFavorsPerDay;
		case SDS::SDSAttribute::kLuck:
			return RE::ActorValue::kLastFlattered;
		case SDS::SDSAttribute::kFaith:
			return RE::ActorValue::kLastBribedIntimidated;
		default:
			return RE::ActorValue::kNone;
		}
	}

	float SDSLeveledAttribute::GetBaseActorValue()
	{
		RE::ActorValueOwner* playerAVs = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
		return playerAVs->GetBaseActorValue(GetAccociatedActorValue());
	}

	void SDSLeveledAttribute::SetBaseActorValue(float val)
	{
		RE::ActorValueOwner* playerAVs = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
		playerAVs->SetBaseActorValue(GetAccociatedActorValue(), val);
	}

	void SDSLeveledAttribute::IncrementAttribute(float val)
	{
		RE::ActorValueOwner* playerAVs = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
		RE::ActorValue accociatedAV = GetAccociatedActorValue(); 
		playerAVs->SetBaseActorValue(accociatedAV, val + playerAVs->GetBaseActorValue(accociatedAV));
	}

	const char* SDSLeveledAttribute::NameByAttribute(SDSAttribute attr)
	{
		switch (attr) {
		case SDS::SDSAttribute::kStrength:
			return "Strength";
		case SDS::SDSAttribute::kIntelligence:
			return "Intelligence";
		case SDS::SDSAttribute::kWillpower:
			return "Willpower";
		case SDS::SDSAttribute::kAgility:
			return "Agility";
		case SDS::SDSAttribute::kSpeed:
			return "Speed";
		case SDS::SDSAttribute::kEndurance:
			return "Endurance";
		case SDS::SDSAttribute::kPersonality:
			return "Personality";
		case SDS::SDSAttribute::kLuck:
			return "Luck";
		case SDS::SDSAttribute::kFaith:
			return "Faith";
		case SDS::SDSAttribute::kNone:
		default:
			return "None";
		}
	}

}
