#include "HandlerLevelUpMenu.h"

#include "Specialization.h"
#include "Settings.h"
#include "ScaleformHelper.h"

#include "HooksCore.h"

namespace SDS
{
	void HandlerLevelUpMenu::OnMenuChange(bool isOpening)
	{
		RE::ActorHandle playerHandle = RE::PlayerCharacter::GetSingleton()->GetHandle();

		if (isOpening) {
			RE::GPtr<RE::GFxMovieView> uiMovie = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;

			// Player ActorValues numeric array
			RE::GFxValue playerAVs;
			// Calculated maximum skill limits array for current level
			RE::GFxValue skillCaps;
			// ActionScript object
			RE::GFxValue levelingArgs;
			// Specialization leveling object
			RE::GFxValue spec;
			// Perk only values
			RE::GFxValue perks;

			uiMovie->CreateObject(&levelingArgs);

			levelingArgs.SetMember("skillLevelupsPerLevel", RE::GFxValue(Settings::iMaxLevelsPerSkillPerPlayerLevel));

			int skillPoints = static_cast<int>(Settings::iSkillPointsPerLevel + playerHandle.get()->GetLevel() * Settings::fSkillPointsLevelMultiplier);
			skillPoints += Settings::Saved->RemainingSkillPoints;

			if (skillPoints > Settings::iSkillPointsCap && Settings::iSkillPointsCap > 0)
				skillPoints = Settings::iSkillPointsCap;

			levelingArgs.SetMember("skillPoints", RE::GFxValue(skillPoints));
			levelingArgs.SetMember("cost0", RE::GFxValue(Settings::iSkillPointCost0));
			levelingArgs.SetMember("cost25", RE::GFxValue(Settings::iSkillPointCost25));
			levelingArgs.SetMember("cost50", RE::GFxValue(Settings::iSkillPointCost50));
			levelingArgs.SetMember("cost75", RE::GFxValue(Settings::iSkillPointCost75));

			uiMovie->CreateArray(&playerAVs);
			ScaleformHelper::GetActorBaseAVs(&playerAVs);
			levelingArgs.SetMember("actorValues", playerAVs);

			uiMovie->CreateArray(&skillCaps);
			ScaleformHelper::GetPlayerSkillCaps(&skillCaps);
			levelingArgs.SetMember("skillCaps", skillCaps);

			uiMovie->CreateObject(&perks);
			ScaleformHelper::GetPlayerPerkValues(&perks);
			levelingArgs.SetMember("perkValues", perks);

			uiMovie->CreateObject(&spec);
			if (Settings::selectedSpecIndex != -1)
				Specialization::Specializations[Settings::selectedSpecIndex]->ToGfxValue(&spec, uiMovie);
			levelingArgs.SetMember("specialization", spec);

			uiMovie->Invoke("_root.LevelUpMenu_mc.SetLevelingSettings", nullptr, &levelingArgs, 1);
		} else {
		}
	}

	void HandlerLevelUpMenu::AddStats()
	{
		if (Settings::bDebugEnabled)
			SKSE::log::info("Processing player stats");

		uint8_t i = 0, skillLevelUps = 0;

		//RE::ActorValue avID = RE::ActorValue::kOneHanded;
		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::GFxValue playerData, dataKey, keyValue(0);

		uiView->Invoke("_root.LevelUpMenu_mc.GetPlayerSelection", &playerData, nullptr, 0);

		if (Settings::bDebugEnabled)
			SKSE::log::info("Leveling data recieved");

		playerData.GetMember("skills", &dataKey);
		for (; i < dataKey.GetArraySize(); i++) {
			dataKey.GetElement(i, &keyValue);
			skillLevelUps = static_cast<uint8_t>(keyValue.GetNumber());

			if (skillLevelUps > 0)
				SDSHooksCore::PlayerCharacter_IncrementSkillHook(player, static_cast<RE::ActorValue>(i + 6), skillLevelUps);
		}

		i = 0;
		playerData.GetMember("attributes", &dataKey);

		float newActorValue = 0.00f;
		uint16_t currentLevel = player->GetLevel() - 1;
		uint16_t attributeValue = 0;

		RE::ActorValueOwner* avOwner = player->AsActorValueOwner();

		for (; i < dataKey.GetArraySize(); i++) {
			dataKey.GetElement(i, &keyValue);
			attributeValue = (uint16_t)keyValue.GetNumber();

			if (attributeValue == 0)
				continue;

			//SKSE::log::info("Incrementing attribute {} by {}", i + 1, attributeValue);
			Settings::LevelingSettings[i].IncrementAttribute(player, attributeValue);

			for (auto avIt = Settings::LevelingSettings[i].actorValues.begin(); avIt < Settings::LevelingSettings[i].actorValues.end(); avIt++) {
				//SKSE::log::info("Incrementing actorValue {} by {}", static_cast<int>(avIt->av), avOwner->GetBaseActorValue(avIt->av));
				if (avIt->av != RE::ActorValue::kNone) {
					newActorValue = (avIt->GetClosest(currentLevel) * attributeValue) + avOwner->GetBaseActorValue(avIt->av);
					avOwner->SetBaseActorValue(avIt->av, newActorValue);
				}
			}
		}

		playerData.GetMember("remainingSkillPoints", &keyValue);
		Settings::Saved->RemainingSkillPoints = static_cast<int>(keyValue.GetNumber());

		if (Settings::bDebugEnabled)
			SKSE::log::info("Player stats processed, remaining skillPoints: {}", Settings::Saved->RemainingSkillPoints);
	}

	void HandlerLevelUpMenu::CompleteLevelUp([[maybe_unused]] const RE::FxDelegateArgs& a_params)
	{
		if (Settings::bDebugEnabled)
			SKSE::log::info("Callback from levelupmenu called");
	}

	/*
		@a_params vector<GFxValue<const char*>>
	*/
	void HandlerLevelUpMenu::CalculatePlayerStatsForAttribute(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == static_cast<int>(SDSAttribute::kFaith));

		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		int playerLevel = player->GetLevel() - 1;
		int paramIndex = 0;

		RE::GFxValue attributeData;
		uiView->CreateObject(&attributeData);

		for (SDSAttribute attribute = SDSAttribute::kStrength; attribute <= SDSAttribute::kPersonality;) {
			paramIndex = static_cast<int>(attribute) - 1;

			if (a_params[paramIndex].GetNumber() > 0)
				Settings::LevelingSettings[paramIndex].ToGFxValue(player, (int)a_params[paramIndex].GetNumber(), playerLevel, &attributeData);

			attribute = static_cast<SDSAttribute>(paramIndex + 2);
		}

		uiView->Invoke("_root.LevelUpMenu_mc.HightlightActorValues", nullptr, &attributeData, 1);
	}

	void HandlerLevelUpMenu::HighlightPlayerCard(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);

		SDSAttribute attribute = SDSAttributeByName(a_params[0].GetString());
		int settingsIndex = static_cast<int>(attribute) - 1;

		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;

		RE::GFxValue callParams;
		uiView->CreateObject(&callParams);

		for (auto it = Settings::LevelingSettings[settingsIndex].actorValues.begin(); it < Settings::LevelingSettings[settingsIndex].actorValues.end(); it++) {
			callParams.SetMember(it->name, RE::GFxValue(0));
		}

		uiView->Invoke("_root.LevelUpMenu_mc.playerCard_mc.HighlightValues", nullptr, &callParams, 1);
	}

}
