#pragma once

#define iTrainingsPerLevelOffsetSE 505391
#define iTrainingsPerLevelOffsetAE 0

namespace SDSHooksCore
{

	void RaceSexMenu_ChangeRace(RE::FxDelegateArgs* callArgs);
	void RaceSexMenu_ChangeSex(RE::FxDelegateArgs* callArgs);
	void RaceSexMenu_ChangeName(RE::FxDelegateArgs* callArgs);

	RE::BSSoundHandle* Character_PlaySound(RE::Character*, RE::BSSoundHandle*, RE::BSFixedString&, int, int);

	int Actor_GetGoldCount(RE::Actor*);
	void PlayerSkills_SetLevel(RE::PlayerCharacter::PlayerSkills*, bool ignoreRemainingExperience);
	void PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* player, RE::ActorValue avID, uint8_t count);
	void PlayerCharacter_RemoveAllItems(RE::PlayerCharacter* player, RE::PlayerCharacter* player2);
	void PlayerCharacter_GiftGold(RE::PlayerCharacter*, RE::Actor*, unsigned int count);
	int GameFunc_UpdatePlayerLevel();

	void MessageBoxData_QueueMessage(RE::MessageBoxData* a_messageBox);
	void MessageBoxMenu_PlaySound(const RE::FxDelegateArgs& a_params);

	RE::BGSSoundDescriptorForm* Game_GetSound_DisabledAction();
	void BGSSoundDescriptorForm_Play(RE::BGSSoundDescriptorForm*);

	//void IMenu_ZoomPC(RE::IMenu *menu, bool inOut);
	void IMenu_ZoomPC(const RE::FxDelegateArgs& a_params);
	void FxResponseArgsList_Clear(RE::FxResponseArgsBase* self);
	void* GfxMovieView_InvokeCallback(RE::GFxMovieView* self, const char* callPath, RE::FxResponseArgsBase* argsList);

	int CalculateTrainingCost(float);
}
