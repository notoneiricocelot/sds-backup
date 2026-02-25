#include "Common.h"

namespace SDS
{
	void ResetSkillUse()
	{
		RE::ActorValueList* playerAVs = RE::ActorValueList::GetSingleton();
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		// disable skill experience gain from skills use
		int avIndex = 0;
		for (RE::ActorValue i = RE::ActorValue::kOneHanded; i <= RE::ActorValue::kEnchanting;) {
			playerAVs->GetActorValue(i)->skill->useMult = 0;

			avIndex = static_cast<int>(i) - 6;
			player->skills->data->skills[avIndex].xp = 0;

			i = static_cast<RE::ActorValue>(static_cast<int>(i) + 1);
		}
	}

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
		for (; attr <= SDSFocus::kStealth;) {
			if (strcmp(a_name, SDSFocusToString(attr)) == 0)
				return attr;

			attr = static_cast<SDSFocus>(static_cast<int>(attr) + 1);
		}

		return SDSFocus::kNone;
	}

	SDSFocus SDSFocusByAV(RE::ActorValue av)
	{
		switch (av) {
		case RE::ActorValue::kOneHanded:
		case RE::ActorValue::kTwoHanded:
		case RE::ActorValue::kBlock:
		case RE::ActorValue::kSmithing:
		case RE::ActorValue::kHeavyArmor:
		case RE::ActorValue::kLightArmor:
			return SDSFocus::kCombat;
		case RE::ActorValue::kSneak:
		case RE::ActorValue::kPickpocket:
		case RE::ActorValue::kArchery:
		case RE::ActorValue::kLockpicking:
		case RE::ActorValue::kSpeech:
		case RE::ActorValue::kAlchemy:
			return SDSFocus::kStealth;
		case RE::ActorValue::kIllusion:
		case RE::ActorValue::kConjuration:
		case RE::ActorValue::kEnchanting:
		case RE::ActorValue::kAlteration:
		case RE::ActorValue::kDestruction:
		case RE::ActorValue::kRestoration:
			return SDSFocus::kStealth;
		default:
			return SDSFocus::kNone;
		}
	}

	SDSAttribute SDSAttributeByName(const char* attr_name)
	{
		SDSAttribute attr = SDSAttribute::kStrength;
		for (; attr <= SDSAttribute::kFaith;) {
			if (strcmp(attr_name, SDSLeveledAttribute::NameByAttribute(attr)) == 0)
				return attr;

			attr = static_cast<SDSAttribute>(static_cast<int>(attr) + 1);
		}
		return SDSAttribute::kNone;
	}

	SDSAttribute SDSAttributeBySkill(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kOneHanded:
		case RE::ActorValue::kTwoHanded:
			return SDSAttribute::kStrength;
		case RE::ActorValue::kBlock:
		case RE::ActorValue::kSmithing:
		case RE::ActorValue::kHeavyArmor:
			return SDSAttribute::kEndurance;
		case RE::ActorValue::kSneak:
		case RE::ActorValue::kPickpocket:
		case RE::ActorValue::kArchery:
			return SDSAttribute::kAgility;
		case RE::ActorValue::kLightArmor:
		case RE::ActorValue::kLockpicking:
			return SDSAttribute::kSpeed;
		case RE::ActorValue::kSpeech:
		case RE::ActorValue::kIllusion:
			return SDSAttribute::kPersonality;
		case RE::ActorValue::kAlchemy:
		case RE::ActorValue::kConjuration:
		case RE::ActorValue::kEnchanting:
			return SDSAttribute::kIntelligence;
		case RE::ActorValue::kAlteration:
		case RE::ActorValue::kDestruction:
		case RE::ActorValue::kRestoration:
			return SDSAttribute::kWillpower;
		default:
			return SDSAttribute::kNone;
		}
	}

	float SDSLeveledAttribute::GetAttributePerkFunctionValue(RE::Character* actor, SDSLeveledValue*)
	{
		// TODO get perk data multiplier
		float perkMultiplier = 0.01f;
		return (1 + (GetBaseActorValue(actor) * perkMultiplier));
	}

	void SDSLeveledAttribute::ToGFxValue(RE::Character* actor, int multiplier, int playerLevel, RE::GFxValue* value)
	{
		double newValue;
		RE::GFxValue avValue;

		for (auto it = this->actorValues.begin(); it < this->actorValues.end(); it++)
		{
			if (it->av == RE::ActorValue::kNone)
			{
				newValue = multiplier * GetAttributePerkFunctionValue(actor, it._Ptr);
			}
			else
			{
				newValue = lower_bound_key<float>(playerLevel, (*it).leveledValues) * multiplier;

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

	float SDSLeveledAttribute::GetBaseActorValue(RE::Character* actor)
	{
		return actor->AsActorValueOwner()->GetBaseActorValue(GetAccociatedActorValue());
	}

	void SDSLeveledAttribute::SetBaseActorValue(RE::Character* actor, float val)
	{
		RE::ActorValueOwner* avOwner = actor->AsActorValueOwner();
		avOwner->SetBaseActorValue(GetAccociatedActorValue(), val);

		for (size_t i = 0; i < actorValues.size(); i++)
		{
			if (actorValues[i].av == RE::ActorValue::kNone)
			{
			}
			else
			{
				// TODO
				// avOwner->SetBaseActorValue(actorValues[i].av, val * actorValues[i].GetClosest(actor->GetLevel()));
				// avOwner->SetActorValue(actorValues[i].av, val * actorValues[i].GetClosest(actor->GetLevel()));
			}
		}
	}

	void SDSLeveledAttribute::IncrementAttribute(RE::Character* actor, float attributeValue)
	{
		if (attributeValue == 0)
			return;

		RE::ActorValueOwner* avOwner = actor->AsActorValueOwner();
		RE::ActorValue accociatedAV = GetAccociatedActorValue(); 
		avOwner->SetBaseActorValue(accociatedAV, attributeValue + avOwner->GetBaseActorValue(accociatedAV));
		
		int currentLevel = actor->GetLevel();
		float newActorValue = 0;

		for (size_t i = 0; i < actorValues.size(); i++)
		{
			if (actorValues[i].av == RE::ActorValue::kNone)
			{

			}
			else
			{
				newActorValue = (lower_bound_key<float>(currentLevel, actorValues[i].leveledValues) * attributeValue) + avOwner->GetBaseActorValue(actorValues[i].av);
				avOwner->SetBaseActorValue(actorValues[i].av, newActorValue);
			}
		}
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

	void PlayerData::AddSkillPoints(SDSFocus focus, int amount)
	{
		switch (focus)
		{
			case SDSFocus::kCombat:
				CombatSkillPoints += amount;
				break;
			case SDSFocus::kMagick:
				MagicSkillPoints += amount;
				break;
			case SDSFocus::kStealth:
				StealthSkillPoints += amount;
				break;
		}
	}

	int PlayerData::GetSkillPoints(SDSFocus focus)
	{
		switch (focus)
		{
			case SDSFocus::kCombat:
				return CombatSkillPoints;
			case SDSFocus::kMagick:
				return MagicSkillPoints;
			case SDSFocus::kStealth:
				return StealthSkillPoints;
		}
		return 0;
	}
}
