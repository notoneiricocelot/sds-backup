#include "LevelUp.h"

#include "Common.h"
#include "Settings.h"

#include "ScaleformHelper.h"

namespace SDS
{
	void LevelUpMenuHandler::OnMenuChange(bool isOpening)
	{
		RE::ActorHandle playerHandle = RE::PlayerCharacter::GetSingleton()->GetHandle();
		auto uiMovie = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;

		if (isOpening) {
			SKSE::log::info("Opening levelup menu");

			// Player ActorValues numeric array
			RE::GFxValue playerAVs;
			// Calculated maximum skill limits array for current level
			RE::GFxValue skillCaps;
			// ActionScript object
			RE::GFxValue levelingArgs;

			uiMovie->CreateObject(&levelingArgs);
			uiMovie->CreateArray(&playerAVs);
			uiMovie->CreateArray(&skillCaps);

			levelingArgs.SetMember("skillLevelupsPerLevel", RE::GFxValue(Settings::iMaxLevelsPerSkillPerPlayerLevel));

			int skillPoints = static_cast<int>(Settings::iSkillPointsPerLevel + playerHandle.get()->GetLevel() * Settings::fSkillPointsLevelMultiplier);
			if (skillPoints > Settings::iSkillPointsCap && Settings::iSkillPointsCap > 0)
				skillPoints = Settings::iSkillPointsCap;

			levelingArgs.SetMember("skillPoints", RE::GFxValue(skillPoints));
			levelingArgs.SetMember("cost0", RE::GFxValue(Settings::iSkillPointCost0));
			levelingArgs.SetMember("cost25", RE::GFxValue(Settings::iSkillPointCost25));
			levelingArgs.SetMember("cost50", RE::GFxValue(Settings::iSkillPointCost50));
			levelingArgs.SetMember("cost75", RE::GFxValue(Settings::iSkillPointCost75));

			ScaleformHelper::GetActorBaseAVs(&playerAVs);
			levelingArgs.SetMember("actorValues", playerAVs);

			ScaleformHelper::GetPlayerSkillCaps(&skillCaps);
			levelingArgs.SetMember("skillCaps", skillCaps);

			uiMovie->Invoke("_root.LevelUpMenu_mc.setLevelingSettings", nullptr, &levelingArgs, 1);
		}
	}
}
