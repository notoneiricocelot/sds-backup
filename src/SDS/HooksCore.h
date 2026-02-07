#pragma once

namespace SDSHooksCore
{

	void RaceSexMenu_ChangeRace(RE::FxDelegateArgs* callArgs);
	void RaceSexMenu_ChangeSex(RE::FxDelegateArgs* callArgs);
	void RaceSexMenu_ChangeName(RE::FxDelegateArgs* callArgs);

	RE::BSSoundHandle* Character_PlaySound(RE::Character*, RE::BSSoundHandle*, RE::BSFixedString&, int, int);

	void PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* player, RE::ActorValue avID, uint8_t count);
	void PlayerCharacter_RemoveAllItems(RE::PlayerCharacter* player, RE::PlayerCharacter* player2);

	void MessageBoxMenu_PlaySound(const RE::FxDelegateArgs& a_params);

	//void IMenu_ZoomPC(RE::IMenu *menu, bool inOut);
	void IMenu_ZoomPC(const RE::FxDelegateArgs& a_params);
	void FxResponseArgsList_Clear(RE::FxResponseArgsBase* self);
	void* GfxMovieView_InvokeCallback(RE::GFxMovieView* self, const char* callPath, RE::FxResponseArgsBase* argsList);

}
