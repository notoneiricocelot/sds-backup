#include "Hooks.h"
#include "Settings.h"
#include "Specialization.h"

#include <typeinfo>

namespace SDS
{

	void LevelUpMenuHookHandlers::AddStats()
	{
		if (Settings::bSSLDebugEnabled)
			SKSE::log::info("Processing player stats");

		uint8_t i = 0, skillLevelUps = 0;

		//RE::ActorValue avID = RE::ActorValue::kOneHanded;
		RE::GPtr<RE::GFxMovieView> uiView = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::GFxValue playerData, dataKey, keyValue(0);

		uiView->Invoke("_root.LevelUpMenu_mc.GetPlayerSelection", &playerData, nullptr, 0);

		if (Settings::bSSLDebugEnabled)
			SKSE::log::info("Leveling data recieved");

		playerData.GetMember("skills", &dataKey);
		for (; i < dataKey.GetArraySize(); i++) {
			dataKey.GetElement(i, &keyValue);
			skillLevelUps = static_cast<uint8_t>(keyValue.GetNumber());

			if (skillLevelUps > 0)
				HUDHooks::PlayerCharacter_IncrementSkillHook(player, static_cast<RE::ActorValue>(i + 6), skillLevelUps);
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
			Settings::LevelingSettings[i].IncrementAttribute(attributeValue);

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

		if (Settings::bSSLDebugEnabled)
			SKSE::log::info("Player stats processed, remaining skillPoints: {}", Settings::Saved->RemainingSkillPoints);
	}

	void LevelUpMenuHookHandlers::CompleteLevelUp([[maybe_unused]] const RE::FxDelegateArgs& a_params)
	{
		if (Settings::bSSLDebugEnabled)
			SKSE::log::info("Callback from levelupmenu called");
	}

	/*
		@a_params vector<GFxValue<const char*>>
	*/
	void LevelUpMenuHookHandlers::CalculatePlayerStatsForAttribute(const RE::FxDelegateArgs& a_params)
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
				Settings::LevelingSettings[paramIndex].ToGFxValue((int)a_params[paramIndex].GetNumber(), playerLevel, &attributeData);

			attribute = static_cast<SDSAttribute>(paramIndex + 2);
		}

		uiView->Invoke("_root.LevelUpMenu_mc.HightlightActorValues", nullptr, &attributeData, 1);
	}

	void LevelUpMenuHookHandlers::HighlightPlayerCard(const RE::FxDelegateArgs& a_params)
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

	void RaceSexMenuHookHandlers::ChangeClass(const RE::FxDelegateArgs& a_params)
	{
		if (a_params.GetArgCount() != 1)
			return;

		const char* a_id = a_params[0].GetString();

		// setup initial attribute values
		for (auto it = Settings::LevelingSettings.begin(); it < Settings::LevelingSettings.end(); it++) {
			it->SetBaseActorValue(0.0f + Settings::iBaseAttributeValue);
		}

		Settings::Saved->SelectedSpecializationID = std::string(a_id);
		if (Settings::bSSLDebugEnabled)
			SKSE::log::info("Class {} selected"sv, a_id);
	}

	void RaceSexMenuHookHandlers::GetNewBonuses(const RE::FxDelegateArgs& a_params)
	{
		if (a_params.GetArgCount() < 2)
			return;

		RE::RaceSexMenu* self = static_cast<RE::RaceSexMenu*>(a_params.GetHandler());
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		const char* specID = a_params[1].GetString();

		if (Settings::bSSLDebugEnabled)
			SKSE::log::info("Updating ui bonuses for spec {}"sv, specID);

		int selectedSpecIndex = Specialization::Find(specID);
		RE::FormID playerRaceID = player->GetRace()->GetFormID();

		RE::FxResponseArgs<8+18> callParams;

		SDSAttribute firstAttribute = Specialization::Specializations[selectedSpecIndex]->GetFirstAttribute();
		SDSAttribute secondAttribute = Specialization::Specializations[selectedSpecIndex]->GetSecondAttribute();
		int bonusResult = 0;

		for (auto it = Settings::LevelingSettings.begin(); it < Settings::LevelingSettings.end(); it++)
		{
			bonusResult = it->racialBonuses[playerRaceID] + Settings::iBaseAttributeValue;
			if (it->attribute == firstAttribute || it->attribute == secondAttribute)
				bonusResult = bonusResult + Settings::iClassBaseAttributeBonus;
			callParams.Add(RE::GFxValue(bonusResult));
		}

		std::array<int, 18> skillBoosts{};
		int i = 0;
		int avIndex = 0;

		for (i = 0; i < 6; i++)
		{
			avIndex = Specialization::Specializations[selectedSpecIndex]->skills[i] - 6;
			skillBoosts[avIndex] = Settings::iClassBaseSkillBonus;
		}

		for (i = 0; i < RE::RACE_DATA::kNumSkillBoosts; i++)
		{
			if (player->GetActorBase()->race->data.skillBoosts[i].skill.get() == RE::ActorValue::kNone)
				continue;

			avIndex = static_cast<int>(player->GetActorBase()->race->data.skillBoosts[i].skill.get()) - 6;
			skillBoosts[avIndex] = skillBoosts[avIndex] + player->GetActorBase()->race->data.skillBoosts[i].bonus;
		}

		for (i = 0; i < 18; i++)
		{
			callParams.Add(RE::GFxValue(skillBoosts[i]));
		}

		CoreHooks::GfxMovieView_InvokeCallback(self->uiMovie.get(), "UpdateBonusesInfo", &callParams);
	}

	// public
	void HUDHooks::PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* referer, RE::ActorValue avID, uint8_t count)
	{
		using func_t = decltype(&HUDHooks::PlayerCharacter_IncrementSkillHook);
		REL::Relocation<func_t> func{ REL::RelocationID(39414, 0) };  // 6a2580
		func(referer, avID, count);
	}

	/* void HUDHooks::IMenu_ZoomPC(RE::IMenu* menu, bool inOut)
	{
		using func_t = decltype(&HUDHooks::IMenu_ZoomPC);
		REL::Relocation<func_t> func{ REL::RelocationID(51539, 0) };  // 8ba360
		//REL::Relocation<func_t> func{ REL::RelocationID(51486, 0) };  // 8B1330
		func(menu, inOut);
	}
	*/

	void HUDHooks::IMenu_ZoomPC(const RE::FxDelegateArgs& a_params)
	{
		using func_t = decltype(&HUDHooks::IMenu_ZoomPC);
		REL::Relocation<func_t> func{ REL::RelocationID(51486, 0) };  // 8B1330
		func(a_params);
	}

	void HUDHooks::MessageBoxMenu_PlaySound(const RE::FxDelegateArgs& a_params)
	{
		using func_t = decltype(&HUDHooks::MessageBoxMenu_PlaySound);
		REL::Relocation<func_t> func{ REL::RelocationID(52051, 0) };  // 8da5c0
		func(a_params);
	}

	/**
		UI hooks
	*/
	void HUDHooks::LevelUpMenu_AcceptProcessorHook(RE::LevelUpMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		_LevelUpMenu_AcceptProcessorHook(a_this, a_processor);
		SKSE::log::info("{}::Accept called", a_this->MENU_NAME);

		a_processor->Process("completeLevelUp", LevelUpMenuHookHandlers::CompleteLevelUp);
		a_processor->Process("calculatePlayerStatsForAttribute", LevelUpMenuHookHandlers::CalculatePlayerStatsForAttribute);
		a_processor->Process("highlightPlayerCard", LevelUpMenuHookHandlers::HighlightPlayerCard);
	}

	void HUDHooks::LevelUpMenu_ConfirmLevelUpHook(RE::LevelUpMenu* a_this, void* a_data)
	{
		LevelUpMenuHookHandlers::AddStats();
		_LevelUpMenu_ConfirmLevelUpHook(a_this, a_data);
	}

	void HUDHooks::RaceSexMenu_AcceptProcessorHook(RE::RaceSexMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		_RaceSexMenu_AcceptProcessorHook(a_this, a_processor);
		SKSE::log::info("{}::Accept called", a_this->MENU_NAME);

		a_processor->Process("ChangeClass", RaceSexMenuHookHandlers::ChangeClass);
		a_processor->Process("GetNewBonuses", RaceSexMenuHookHandlers::GetNewBonuses);
	}

	/**
		Game function cals
	*/

	void HUDHooks::RaceSexMenu_ChangeRace(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&HUDHooks::RaceSexMenu_ChangeRace);
		REL::Relocation<func_t> func{ REL::RelocationID(51489, 0) };  // 8b13f0
		func(callArgs);
	}

	void HUDHooks::RaceSexMenu_ChangeSex(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&HUDHooks::RaceSexMenu_ChangeSex);
		REL::Relocation<func_t> func{ REL::RelocationID(51490, 0) };  // 8b1640
		func(callArgs);
	}

	void HUDHooks::RaceSexMenu_ChangeName(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&HUDHooks::RaceSexMenu_ChangeName);
		REL::Relocation<func_t> func{ REL::RelocationID(51488, 0) };  // 8b1360
		func(callArgs);
	}

	void CoreHooks::FxResponseArgsList_Clear(RE::FxResponseArgsBase* self)
	{
		using func_t = decltype(&CoreHooks::FxResponseArgsList_Clear);
		REL::Relocation<func_t> func{ REL::RelocationID(50041, 0) };  // 855220
		func(self);
	}

	void* CoreHooks::GfxMovieView_InvokeCallback(RE::GFxMovieView* self, const char* callPath, RE::FxResponseArgsBase* argsList)
	{
		using func_t = decltype(&CoreHooks::GfxMovieView_InvokeCallback);
		REL::Relocation<func_t> func{ REL::RelocationID(80520, 0) };  // ed6ac0
		return func(self, callPath, argsList);
	}
}
