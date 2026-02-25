#include "HandlerTrainingMenu.h"

#include "Common.h"
#include "ScaleformHelper.h"
#include "Settings.h"

#include "HooksCore.h"

namespace SDSHandlerTrainingMenu
{
	void UpdatePlayerCard(RE::TrainingMenu* aMenu)
	{
		RE::FxResponseArgs<7> callArgs;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		int skillCap = SDS::GetSkillLevelCap(player, aMenu->skill);

		if (skillCap > aMenu->trainer->GetActorBase()->npcClass->data.maximumTrainingLevel)
			skillCap = aMenu->trainer->GetActorBase()->npcClass->data.maximumTrainingLevel;
		callArgs.Add(RE::GFxValue(skillCap));

		int skillLevel = static_cast<int>(player->GetBaseActorValue(aMenu->skill));
		callArgs.Add(RE::GFxValue(SDS::GetSkillpointCost(skillLevel)));

		int attrID = SDS::SDSAttributeBySkill(aMenu->skill) - 1;
		int attrValue = static_cast<int>(SDS::Settings::LevelingSettings[attrID].GetBaseActorValue(player));
		callArgs.Add(RE::GFxValue(attrValue));
		// Creating copy bc sometimes gfx::value wont display pointer content
		callArgs.Add(RE::GFxValue(std::string(SDS::Settings::LevelingSettings[attrID].name)));
		callArgs.Add(RE::GFxValue(SDS::GetSkillpointCost(skillLevel - 1)));
		callArgs.Add(RE::GFxValue(static_cast<int>(aMenu->skill)));

		SDS::SDSFocus skillFocus = SDS::SDSFocusByAV(aMenu->skill);
		int focusSkillPoints = SDS::Settings::Saved->GetSkillPoints(skillFocus);

		callArgs.Add(RE::GFxValue(focusSkillPoints));
		SDSHooksCore::GfxMovieView_InvokeCallback(aMenu->uiMovie.get(), "UpdateCard", &callArgs);
	}

	void TrainFocus(const RE::FxDelegateArgs &callArgs)
	{
		RE::TrainingMenu* menu = (RE::TrainingMenu*) callArgs.GetHandler();
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		unsigned int* iTrainingNumAllowedPerLevel = (unsigned int*) RELOCATION_ID(iTrainingsPerLevelOffsetSE, iTrainingsPerLevelOffsetAE).address();  // 1de20f8
		const char** debugString = 0;
		RE::BGSSoundDescriptorForm* actionSound;

		if (player->skillTrainingsThisLevel >= *iTrainingNumAllowedPerLevel)
		{
			debugString = (const char**) RELOCATION_ID(506987, 0).address(); // 1DE8108
debugMsg:
			RE::DebugNotification(debugString[1], 0, 1);
			actionSound = SDSHooksCore::Game_GetSound_DisabledAction();
			SDSHooksCore::BGSSoundDescriptorForm_Play(actionSound);
			return;
		}

		float baseAV = player->AsActorValueOwner()->GetBaseActorValue(menu->skill);
		int trainingCost = SDSHooksCore::CalculateTrainingCost(baseAV);

		if (SDSHooksCore::Actor_GetGoldCount(player) < trainingCost)
		{
			debugString = (const char**) RELOCATION_ID(506989, 0).address(); // 1DE8138
			goto debugMsg;
			return;
		}

		SDS::SDSFocus skillFocus = SDS::SDSFocusByAV(menu->skill);
		int skillPoints = SDS::GetSkillpointCost(static_cast<int>(baseAV));

		SDSHooksCore::PlayerCharacter_GiftGold(player, menu->trainer, static_cast<unsigned int>(trainingCost));
		SDS::Settings::Saved->AddSkillPoints(skillFocus, skillPoints);
		player->skillTrainingsThisLevel += 1;

		TrainingMenu_UpdateCardText(menu);
		UpdatePlayerCard(menu);
	}

	void AddHooks()
	{
		REL::Relocation<uintptr_t> trainHook{ RELOCATION_ID(51793, 0) };      // 8CE8E0 + E3/call
		REL::Relocation<uintptr_t> updateCardHook{ RELOCATION_ID(51792, 0) }; // 8ce740 + 17C/call

		SKSE::Trampoline& trampoline = SKSE::GetTrampoline();
		_TrainingMenu_UpdateText_Train = trampoline.write_call<5>(trainHook.address() + RELOCATION_OFFSET(0xE3, 0x0), &TrainingMenu_UpdateText_Train);
		_TrainingMenu_UpdateText_UpdateCard = trampoline.write_call<5>(updateCardHook.address() + RELOCATION_OFFSET(0x17C, 0x0), &TrainingMenu_UpdateText_UpdateCard);

		REL::Relocation<std::uintptr_t> vTable1(RE::VTABLE_TrainingMenu[0]);
		_TrainingMenu_Accept = vTable1.write_vfunc(0x1, &TrainingMenu_Accept);
		_TrainingMenu_ProcessMessage = vTable1.write_vfunc(0x4, &TrainingMenu_ProcessMessage);
	}

	void TrainingMenu_UpdateCardText(RE::TrainingMenu* aMenu)
	{
		using func_t = decltype(&TrainingMenu_UpdateCardText);
		REL::Relocation<func_t> func{ REL::RelocationID(51794, 0) };  // 8CEA30
		return func(aMenu);
	}

	void TrainingMenu_UpdateText_Train(RE::TrainingMenu* aSelf)
	{
		_TrainingMenu_UpdateText_Train(aSelf);
		UpdatePlayerCard(aSelf);
	}

	void TrainingMenu_UpdateText_UpdateCard(RE::TrainingMenu* aSelf)
	{
		_TrainingMenu_UpdateText_UpdateCard(aSelf);
		UpdatePlayerCard(aSelf);
	}

	void TrainingMenu_Accept(RE::TrainingMenu* aSelf, RE::FxDelegateHandler::CallbackProcessor* processor)
	{
		_TrainingMenu_Accept(aSelf, processor);
		processor->Process("TrainFocus", TrainFocus);
	}

	RE::UI_MESSAGE_RESULTS TrainingMenu_ProcessMessage(RE::TrainingMenu* aMenu, RE::UIMessage* aMsg)
	{
		return _TrainingMenu_ProcessMessage(aMenu, aMsg);
	}
}
