#include "Common.h"

namespace SDS
{
	void ScaleformHelper::FillPlayerObject(RE::GFxValue* playerObject)
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

	void ScaleformHelper::FillBottomBarObjectDefault(RE::GFxValue* bottombarObject)
	{
		RE::GFxValue itemType;
		itemType.SetNumber(static_cast<int>(InventoryDefine::kArmor));

		bottombarObject->SetMember("type", itemType);
	}
}
