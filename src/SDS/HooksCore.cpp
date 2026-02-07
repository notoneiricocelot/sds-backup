#include "HooksCore.h"

namespace SDSHooksCore
{

	void FxResponseArgsList_Clear(RE::FxResponseArgsBase* self)
	{
		using func_t = decltype(&FxResponseArgsList_Clear);
		REL::Relocation<func_t> func{ REL::RelocationID(50041, 0) };  // 855220
		func(self);
	}

	void* GfxMovieView_InvokeCallback(RE::GFxMovieView* self, const char* callPath, RE::FxResponseArgsBase* argsList)
	{
		using func_t = decltype(&GfxMovieView_InvokeCallback);
		REL::Relocation<func_t> func{ REL::RelocationID(80520, 0) };  // ed6ac0
		return func(self, callPath, argsList);
	}

	void RaceSexMenu_ChangeRace(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&RaceSexMenu_ChangeRace);
		REL::Relocation<func_t> func{ REL::RelocationID(51489, 0) };  // 8b13f0
		func(callArgs);
	}

	void RaceSexMenu_ChangeSex(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&RaceSexMenu_ChangeSex);
		REL::Relocation<func_t> func{ REL::RelocationID(51490, 0) };  // 8b1640
		func(callArgs);
	}

	void RaceSexMenu_ChangeName(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&RaceSexMenu_ChangeName);
		REL::Relocation<func_t> func{ REL::RelocationID(51488, 0) };  // 8b1360
		func(callArgs);
	}

	RE::BSSoundHandle* Character_PlaySound(RE::Character* actor, RE::BSSoundHandle* soundHandle, RE::BSFixedString &soundName, int notUsed, int nearActor)
	{
		using func_t = decltype(&Character_PlaySound);
		REL::Relocation<func_t> func{ REL::RelocationID(36731, 0) };  // 5fc110
		return func(actor, soundHandle, soundName, notUsed, nearActor);
	}

	// public
	void PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* referer, RE::ActorValue avID, uint8_t count)
	{
		using func_t = decltype(&PlayerCharacter_IncrementSkillHook);
		REL::Relocation<func_t> func{ REL::RelocationID(39414, 0) };  // 6a2580
		func(referer, avID, count);
	}

	void PlayerCharacter_RemoveAllItems(RE::PlayerCharacter* player, RE::PlayerCharacter* player2)
	{
		using func_t = decltype(&PlayerCharacter_RemoveAllItems);
		REL::Relocation<func_t> func{ REL::RelocationID(39436, 0) };  // 6a3940
		func(player, player2);

	}

	/* void IMenu_ZoomPC(RE::IMenu* menu, bool inOut)
	{
		using func_t = decltype(&IMenu_ZoomPC);
		REL::Relocation<func_t> func{ REL::RelocationID(51539, 0) };  // 8ba360
		//REL::Relocation<func_t> func{ REL::RelocationID(51486, 0) };  // 8B1330
		func(menu, inOut);
	}
	*/

	void IMenu_ZoomPC(const RE::FxDelegateArgs& a_params)
	{
		using func_t = decltype(&IMenu_ZoomPC);
		REL::Relocation<func_t> func{ REL::RelocationID(51486, 0) };  // 8B1330
		func(a_params);
	}

	void MessageBoxMenu_PlaySound(const RE::FxDelegateArgs& a_params)
	{
		using func_t = decltype(&MessageBoxMenu_PlaySound);
		REL::Relocation<func_t> func{ REL::RelocationID(52051, 0) };  // 8da5c0
		func(a_params);
	}

}
