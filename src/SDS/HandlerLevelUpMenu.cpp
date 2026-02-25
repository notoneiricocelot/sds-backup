#include "HandlerLevelUpMenu.h"

#include "Specialization.h"
#include "Settings.h"
#include "ScaleformHelper.h"

#include "HooksCore.h"

namespace SDSHandlerLevelUpMenu
{
	void ReloadUI()
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
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
		// Focus based skill points
		RE::GFxValue trainingSkillPoints;

		uiMovie->CreateObject(&levelingArgs);

		uint32_t timesTrained = player->skillTrainingsThisLevel;
		int oldLevel = player->GetLevel();
		SDSHooksCore::GameFunc_UpdatePlayerLevel();
		player->skillTrainingsThisLevel = timesTrained;

		int newLevel = player->GetLevel();
		int levelDiff = std::abs(newLevel - oldLevel);

		int attributePoints = SDS::Settings::Saved->RemainingAttributes;
		int skillPoints = SDS::Settings::Saved->RemainingSkillPoints;
		if (levelDiff == 0)
		{
			levelDiff = 1;
		}
		else
		{
			unsigned int* iTrainingNumAllowedPerLevel = (unsigned int*)RELOCATION_ID(iTrainingsPerLevelOffsetSE, iTrainingsPerLevelOffsetAE).address();

			for (int i = oldLevel; i < newLevel; i++)
			{
				*iTrainingNumAllowedPerLevel += SDS::lower_bound_key<int>(i + 1, SDS::Settings::trainingData);
				skillPoints += SDS::Settings::iSkillPointsPerLevel + i * static_cast<int>(std::floorf(SDS::Settings::fSkillPointsLevelMultiplier));
			}

			attributePoints += static_cast<int>(SDS::Settings::iAttributePointsPerLevel * levelDiff);
		}

		if (skillPoints > (SDS::Settings::iSkillPointsCap * levelDiff) && SDS::Settings::iSkillPointsCap > 0)
		{
			skillPoints = (SDS::Settings::iSkillPointsCap * levelDiff);
		}

		SDS::Settings::Saved->RemainingSkillPoints = skillPoints;
		SDS::Settings::Saved->RemainingAttributes = attributePoints;

		levelingArgs.SetMember("attributePoints", RE::GFxValue(attributePoints));
		levelingArgs.SetMember("attrCap", RE::GFxValue(SDS::Settings::iAttributeCap));

		levelingArgs.SetMember("skillLevelupsPerLevel", RE::GFxValue(SDS::Settings::iMaxLevelsPerSkillPerPlayerLevel * levelDiff));
		SKSE::log::info("Skillpoints levelup limit: {}", SDS::Settings::iMaxLevelsPerSkillPerPlayerLevel * levelDiff);
		levelingArgs.SetMember("skillPoints", RE::GFxValue(skillPoints));

		levelingArgs.SetMember("levelDiff", RE::GFxValue(levelDiff));

		if (SDS::Settings::bDebugEnabled)
			SKSE::log::info("Leveling skills data: [{}, {}, {}, {}, {}]", oldLevel, levelDiff, newLevel, skillPoints, attributePoints);

		uiMovie->CreateArray(&trainingSkillPoints);
		trainingSkillPoints.PushBack(SDS::Settings::Saved->CombatSkillPoints);
		trainingSkillPoints.PushBack(SDS::Settings::Saved->StealthSkillPoints);
		trainingSkillPoints.PushBack(SDS::Settings::Saved->MagicSkillPoints);
		levelingArgs.SetMember("trainingData", trainingSkillPoints);

		levelingArgs.SetMember("cost0", RE::GFxValue(SDS::Settings::iSkillPointCost0));
		levelingArgs.SetMember("cost25", RE::GFxValue(SDS::Settings::iSkillPointCost25));
		levelingArgs.SetMember("cost50", RE::GFxValue(SDS::Settings::iSkillPointCost50));
		levelingArgs.SetMember("cost75", RE::GFxValue(SDS::Settings::iSkillPointCost75));

		uiMovie->CreateArray(&playerAVs);
		SDS::ScaleformHelper::GetActorBaseAVs(&playerAVs);
		levelingArgs.SetMember("actorValues", playerAVs);

		uiMovie->CreateArray(&skillCaps);
		SDS::ScaleformHelper::GetPlayerSkillCaps(&skillCaps);
		levelingArgs.SetMember("skillCaps", skillCaps);

		uiMovie->CreateObject(&perks);
		SDS::ScaleformHelper::GetPlayerPerkValues(&perks);
		levelingArgs.SetMember("perkValues", perks);

		uiMovie->CreateObject(&spec);
		if (SDS::Settings::Saved->PlayerSpecIndex != -1)
		{
			SDS::Specialization::Specializations[SDS::Settings::Saved->PlayerSpecIndex]->ToGfxValue(&spec, uiMovie);
		}
		levelingArgs.SetMember("specialization", spec);

		uiMovie->Invoke("_root.LevelUpMenu_mc.SetLevelingSettings", nullptr, &levelingArgs, 1);
	}

	void AddStats()
	{
		if (SDS::Settings::bDebugEnabled)
			SKSE::log::info("Processing player stats");

		uint8_t i = 0, skillLevelUps = 0;

		//RE::ActorValue avID = RE::ActorValue::kOneHanded;
		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::GFxValue playerData, dataKey, keyValue(0);

		uiView->Invoke("_root.LevelUpMenu_mc.GetPlayerSelection", &playerData, nullptr, 0);

		playerData.GetMember("skills", &dataKey);
		for (; i < dataKey.GetArraySize(); i++) {
			dataKey.GetElement(i, &keyValue);
			skillLevelUps = static_cast<uint8_t>(keyValue.GetNumber());

			if (skillLevelUps > 0)
				SDSHooksCore::PlayerCharacter_IncrementSkillHook(player, static_cast<RE::ActorValue>(i + 6), skillLevelUps);
		}

		i = 0;
		playerData.GetMember("attributes", &dataKey);

		uint16_t attributeValue = 0;

		for (; i < dataKey.GetArraySize(); i++)
		{
			dataKey.GetElement(i, &keyValue);
			attributeValue = static_cast<uint16_t>(keyValue.GetNumber());

			if (attributeValue == 0)
				continue;

			SDS::Settings::LevelingSettings[i].IncrementAttribute(player, attributeValue);
		}

		playerData.GetMember("remainingSkillPoints", &keyValue);
		SDS::Settings::Saved->RemainingSkillPoints = static_cast<int>(keyValue.GetNumber());
		playerData.GetMember("remainingAttributes", &keyValue);
		SDS::Settings::Saved->RemainingAttributes = static_cast<int>(keyValue.GetNumber());

		playerData.GetMember("trainingData", &dataKey);
		dataKey.GetElement(0, &keyValue);
		SDS::Settings::Saved->CombatSkillPoints = static_cast<int>(keyValue.GetNumber());
		dataKey.GetElement(1, &keyValue);
		SDS::Settings::Saved->StealthSkillPoints = static_cast<int>(keyValue.GetNumber());
		dataKey.GetElement(2, &keyValue);
		SDS::Settings::Saved->MagicSkillPoints = static_cast<int>(keyValue.GetNumber());

		if (SDS::Settings::bDebugEnabled)
			SKSE::log::info("Player stats processed, remaining skillPoints: {}", SDS::Settings::Saved->RemainingSkillPoints);
	}

	ConfirmLevelUpMessageCallback::ConfirmLevelUpMessageCallback()
	{
	}

	void ConfirmLevelUpMessageCallback::Run(RE::IMessageBoxCallback::Message response)
	{
		switch (static_cast<int>(response))
		{
		case 4:
			AddStats();
			ReloadUI();
		}

		RE::GPtr<RE::IMenu> menu = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME);
		menu->uiMovie->SetVariable("_root.LevelUpMenu_mc._canLevelUp", RE::GFxValue(true));
		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MessageBoxMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}

	void OnMenuChange(bool isOpening)
	{
		if (isOpening)
		{
			ReloadUI();
		} else
		{
		}
	}

	void CompleteLevelUp(const RE::FxDelegateArgs&)
	{
		RE::GameSettingCollection* gameSettings = RE::GameSettingCollection::GetSingleton();
		
		RE::MessageBoxData* msgBox = SDS::NewMessageBox(gameSettings->GetSetting("sConfirmAttribute")->GetString());
		msgBox->buttonText.push_back(gameSettings->GetSetting("sOk")->GetString());
		msgBox->buttonText.push_back(gameSettings->GetSetting("sCancel")->GetString());
		msgBox->callback = RE::BSTSmartPointer<RE::IMessageBoxCallback>{ new ConfirmLevelUpMessageCallback() };
		SDSHooksCore::MessageBoxData_QueueMessage(msgBox);
	}

	/*
		@a_params vector<GFxValue<const char*>>
	*/
	void CalculatePlayerStatsForAttribute(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == static_cast<int>(SDS::SDSAttribute::kFaith));

		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		int playerLevel = player->GetLevel() - 1;
		int paramIndex = 0;

		RE::GFxValue attributeData;
		uiView->CreateObject(&attributeData);

		for (SDS::SDSAttribute attribute = SDS::SDSAttribute::kStrength; attribute <= SDS::SDSAttribute::kPersonality;) {
			paramIndex = static_cast<int>(attribute) - 1;

			if (a_params[paramIndex].GetNumber() > 0)
				SDS::Settings::LevelingSettings[paramIndex].ToGFxValue(player, (int)a_params[paramIndex].GetNumber(), playerLevel, &attributeData);

			attribute = static_cast<SDS::SDSAttribute>(paramIndex + 2);
		}

		uiView->Invoke("_root.LevelUpMenu_mc.HightlightActorValues", nullptr, &attributeData, 1);
	}

	void HighlightPlayerCard(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);

		SDS::SDSAttribute attribute = SDS::SDSAttributeByName(a_params[0].GetString());
		int settingsIndex = static_cast<int>(attribute) - 1;

		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;

		RE::GFxValue callParams;
		uiView->CreateObject(&callParams);

		for (auto it = SDS::Settings::LevelingSettings[settingsIndex].actorValues.begin(); it < SDS::Settings::LevelingSettings[settingsIndex].actorValues.end(); it++) {
			callParams.SetMember(it->name, RE::GFxValue(0));
		}

		uiView->Invoke("_root.LevelUpMenu_mc.playerCard_mc.HighlightValues", nullptr, &callParams, 1);
	}

	void CloseMenu(const RE::FxDelegateArgs&)
	{
		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::LevelUpMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}

	void AddHooks()
	{
		REL::Relocation<uintptr_t> addMessage_hook{ RELOCATION_ID(51638, 0) };  // 8BF360 + F23/call

		SKSE::Trampoline& trampoline = SKSE::GetTrampoline();
		_StatsMenu_IsLevelUpAvailable = trampoline.write_call<5>(addMessage_hook.address() + RELOCATION_OFFSET(0xF23, 0x0), &StatsMenu_IsLevelUpAvailable);
	}

	bool StatsMenu_IsLevelUpAvailable(RE::PlayerCharacter::PlayerSkills* playerSkills)
	{
		_StatsMenu_IsLevelUpAvailable(playerSkills);
		return false;
	}

}
