#include "ScaleformHelper.h"

#include "Common.h"
#include "Settings.h"

namespace SDS
{
	void ScaleformHelper::GetPlayerObject(RE::GFxValue* playerObject)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto actorValueOwner = player->AsActorValueOwner();

		float maxHealth, maxStamina, maxMagicka;
		RE::GFxValue playerValues[13];

		playerValues[0].SetNumber(actorValueOwner->GetActorValue(AV::kDamageResist));
		playerValues[1].SetNumber(actorValueOwner->GetActorValue(AV::kHealth));
		playerValues[2].SetNumber(actorValueOwner->GetActorValue(AV::kMagicka));
		playerValues[3].SetNumber(actorValueOwner->GetActorValue(AV::kStamina));
		playerValues[4].SetNumber(actorValueOwner->GetActorValue(AV::kInventoryWeight));
		playerValues[5].SetNumber(actorValueOwner->GetActorValue(AV::kCarryWeight));

		maxHealth = actorValueOwner->GetPermanentActorValue(AV::kHealth) + player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, AV::kHealth);
		maxMagicka = actorValueOwner->GetPermanentActorValue(AV::kMagicka) + player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, AV::kMagicka);
		maxStamina = actorValueOwner->GetPermanentActorValue(AV::kStamina) + player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, AV::kStamina);
		playerValues[7].SetNumber(maxHealth);
		playerValues[8].SetNumber(maxMagicka);
		playerValues[9].SetNumber(maxStamina);

		playerValues[10].SetString("#8a0101");
		playerValues[11].SetString("#030a91");
		playerValues[12].SetString("#025909");

		//playerValues[6].SetNumber(player->GetGoldAmount());
		//playerObject.SetMember("gold", playerValues[6]);

		playerObject->SetMember("armor", playerValues[0]);
		playerObject->SetMember("health", playerValues[1]);
		playerObject->SetMember("magicka", playerValues[2]);
		playerObject->SetMember("stamina", playerValues[3]);
		playerObject->SetMember("encumbrance", playerValues[4]);
		playerObject->SetMember("maxEncumbrance", playerValues[5]);

		playerObject->SetMember("maxHealth", playerValues[7]);
		playerObject->SetMember("maxMagicka", playerValues[8]);
		playerObject->SetMember("maxStamina", playerValues[9]);

		playerObject->SetMember("healthColor", playerValues[10]);
		playerObject->SetMember("magickaColor", playerValues[11]);
		playerObject->SetMember("staminaColor", playerValues[12]);
	}

	void ScaleformHelper::GetBottomBarObjectDefault(RE::GFxValue* bottombarObject)
	{
		RE::GFxValue itemType;
		itemType.SetNumber(static_cast<int>(InventoryDefine::kArmor));

		bottombarObject->SetMember("type", itemType);
	}

	void ScaleformHelper::GetActorBaseAVs(RE::GFxValue* av_list)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto actorValueOwner = player->AsActorValueOwner();

		for (AV i = AV::kAggression; i <= AV::kReflectDamage;) {
			av_list->PushBack(static_cast<int>(actorValueOwner->GetBaseActorValue(i)));

			i = static_cast<AV>(static_cast<int>(i) + 1);
		}
	}

	void ScaleformHelper::GetPlayerSkillCaps(RE::GFxValue* av_skills)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		int baseCap = static_cast<int>(Settings::fSkillCapBase + Settings::fSkillCapMult * player->GetLevel());

		int racialBoostIndex = 0;
		RE::RACE_DATA::SkillBoost* boost = nullptr;
		auto skillBoosts = player->GetRace()->data.skillBoosts;

		for (AV i = AV::kOneHanded; i <= AV::kEnchanting;) {
			av_skills->PushBack(baseCap);
			i = static_cast<AV>(static_cast<int>(i) + 1);
		}

		if (Settings::bUseRacialCaps) {
			for (int i = 0; i < RE::RACE_DATA::kNumSkillBoosts; i++) {
				boost = &skillBoosts[i];
				if (boost) {
					racialBoostIndex = static_cast<int>(boost->skill.get()) - 6;
					av_skills->SetElement(racialBoostIndex, baseCap + boost->bonus);
				}
			}
		}
	}
}
