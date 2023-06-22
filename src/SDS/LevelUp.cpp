#include "LevelUp.h"

#include "Common.h"
#include "Specialization.h"
#include "Settings.h"
#include "Hooks.h"

#include "ScaleformHelper.h"

namespace SDS
{
	void LevelUpMenuHandler::OnMenuChange(bool isOpening)
	{
		RE::ActorHandle playerHandle = RE::PlayerCharacter::GetSingleton()->GetHandle();

		if (isOpening) {
			RE::GPtr<RE::GFxMovieView> uiMovie = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;

			SKSE::log::info("Opening levelup menu");

			// Player ActorValues numeric array
			RE::GFxValue playerAVs;
			// Calculated maximum skill limits array for current level
			RE::GFxValue skillCaps;
			// ActionScript object
			RE::GFxValue levelingArgs;
			// Specialization leveling object
			RE::GFxValue spec;
			// Player attributes
			RE::GFxValue attributes;

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

			uiMovie->CreateObject(&spec);
			if (Settings::selectedSpecIndex != -1)
				Specialization::Specializations[Settings::selectedSpecIndex]->ToGfxValue(&spec, uiMovie);
			levelingArgs.SetMember("specialization", spec);

			uiMovie->CreateArray(&attributes);
			ScaleformHelper::GetAttributes(&attributes);
			levelingArgs.SetMember("attributes", attributes);

			uiMovie->Invoke("_root.LevelUpMenu_mc.setLevelingSettings", nullptr, &levelingArgs, 1);
		}
		else {

		}
	}

	void LevelUpMenuHandler::AddStats()
	{
		SKSE::log::info("Processing player stats");

		uint8_t i = 0, skillLevelUps = 0;

		//RE::ActorValue avID = RE::ActorValue::kOneHanded;
		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::GFxValue playerData, dataKey, keyValue(0);


		uiView->Invoke("_root.LevelUpMenu_mc.getPlayerSelection", &playerData, nullptr, 0);
		SKSE::log::info("Leveling data recieved");

		playerData.GetMember("skills", &dataKey);
		for (; i < dataKey.GetArraySize(); i++)
		{
			dataKey.GetElement(i, &keyValue);
			skillLevelUps = static_cast<uint8_t>(keyValue.GetNumber());

			if (skillLevelUps > 0)
				HUDHooks::PlayerCharacter_IncrementSkillHook(player, i + 6, skillLevelUps);
		}

		i = 0;
		playerData.GetMember("attributes", &dataKey);
		for (; i < dataKey.GetArraySize(); i++)
		{
			// TODO
			dataKey.GetElement(i, &keyValue);
		}

		playerData.GetMember("remainingSkillPoints", &keyValue);
		Settings::Saved->RemainingSkillPoints = static_cast<int>(keyValue.GetNumber());
		SKSE::log::info("Player stats processed, remaining skillPoints: {}", Settings::Saved->RemainingSkillPoints);
	}

	void LevelUpMenuHandler::CompleteLevelUp([[maybe_unused]] const RE::FxDelegateArgs& a_params)
	{
		SKSE::log::info("Callback from levelupmenu called");
	}

}
