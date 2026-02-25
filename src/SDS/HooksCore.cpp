#include "HooksCore.h"

namespace SDSHooksCore
{

	void FxResponseArgsList_Clear(RE::FxResponseArgsBase* self)
	{
		using func_t = decltype(&FxResponseArgsList_Clear);
		REL::Relocation<func_t> func{ REL::RelocationID(50041, 0) };  // 855220
		return func(self);
	}

	void* GfxMovieView_InvokeCallback(RE::GFxMovieView* self, const char* callPath, RE::FxResponseArgsBase* argsList)
	{
		using func_t = decltype(&GfxMovieView_InvokeCallback);
		REL::Relocation<func_t> func{ REL::RelocationID(80520, 0) };  // ed6ac0
		return func(self, callPath, argsList);
	}

	int CalculateTrainingCost(float avValue)
	{
		using func_t = decltype(&CalculateTrainingCost);
		REL::Relocation<func_t> func{ REL::RelocationID(25918, 0) };  // 3C1180
		return func(avValue);
	}

	void RaceSexMenu_ChangeRace(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&RaceSexMenu_ChangeRace);
		REL::Relocation<func_t> func{ REL::RelocationID(51489, 0) };  // 8b13f0
		return func(callArgs);
	}

	void RaceSexMenu_ChangeSex(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&RaceSexMenu_ChangeSex);
		REL::Relocation<func_t> func{ REL::RelocationID(51490, 0) };  // 8b1640
		return func(callArgs);
	}

	void RaceSexMenu_ChangeName(RE::FxDelegateArgs* callArgs)
	{
		using func_t = decltype(&RaceSexMenu_ChangeName);
		REL::Relocation<func_t> func{ REL::RelocationID(51488, 0) };  // 8b1360
		return func(callArgs);
	}

	RE::BSSoundHandle* Character_PlaySound(RE::Character* actor, RE::BSSoundHandle* soundHandle, RE::BSFixedString &soundName, int notUsed, int nearActor)
	{
		using func_t = decltype(&Character_PlaySound);
		REL::Relocation<func_t> func{ REL::RelocationID(36731, 0) };  // 5fc110
		return func(actor, soundHandle, soundName, notUsed, nearActor);
	}

	int Actor_GetGoldCount(RE::Actor* actor)
	{
		using func_t = decltype(&Actor_GetGoldCount);
		REL::Relocation<func_t> func{ REL::RelocationID(36527, 0) };  // 5E72F0
		return func(actor);
	}

	void PlayerSkills_SetLevel(RE::PlayerCharacter::PlayerSkills* skills, bool ignoreRemainingExperience)
	{
		using func_t = decltype(&PlayerSkills_SetLevel);
		REL::Relocation<func_t> func{ REL::RelocationID(40560, 0) };  // 6e6740
		return func(skills, ignoreRemainingExperience);
	}

	// public
	void PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* referer, RE::ActorValue avID, uint8_t count)
	{
		using func_t = decltype(&PlayerCharacter_IncrementSkillHook);
		REL::Relocation<func_t> func{ REL::RelocationID(39414, 0) };  // 6a2580
		return func(referer, avID, count);
	}

	void PlayerCharacter_RemoveAllItems(RE::PlayerCharacter* player, RE::PlayerCharacter* player2)
	{
		using func_t = decltype(&PlayerCharacter_RemoveAllItems);
		REL::Relocation<func_t> func{ REL::RelocationID(39436, 0) };  // 6a3940
		return func(player, player2);
	}

	void PlayerCharacter_GiftGold(RE::PlayerCharacter* player, RE::Actor* actorTo, unsigned int count)
	{
		using func_t = decltype(&PlayerCharacter_GiftGold);
		REL::Relocation<func_t> func{ REL::RelocationID(36511, 0) };  // 5e5dd0
		return func(player, actorTo, count);
	}

	int GameFunc_UpdatePlayerLevel()
	{
		using func_t = decltype(&GameFunc_UpdatePlayerLevel);
		REL::Relocation<func_t> func{ REL::RelocationID(21887, 0) };  // 300210
		return func();
	}

	/* void IMenu_ZoomPC(RE::IMenu* menu, bool inOut)
	{
		using func_t = decltype(&IMenu_ZoomPC);
		// REL::Relocation<func_t> func{ REL::RelocationID(51539, 0) };  // 8ba360
		// REL::Relocation<func_t> func{ REL::RelocationID(51486, 0) };  // 8B1330
		func(menu, inOut);
	}
	*/

	void IMenu_ZoomPC(const RE::FxDelegateArgs& a_params)
	{
		using func_t = decltype(&IMenu_ZoomPC);
		REL::Relocation<func_t> func{ REL::RelocationID(51486, 0) };  // 8B1330
		return func(a_params);
	}

	void MessageBoxData_QueueMessage(RE::MessageBoxData* a_messageBox)
	{
		using func_t = decltype(&MessageBoxData_QueueMessage);
		REL::Relocation<func_t> func{ REL::RelocationID(51422, 0) };  // 8AB5C0
		return func(a_messageBox);
	}

	void MessageBoxMenu_PlaySound(const RE::FxDelegateArgs& a_params)
	{
		using func_t = decltype(&MessageBoxMenu_PlaySound);
		REL::Relocation<func_t> func{ REL::RelocationID(52051, 0) };  // 8da5c0
		return func(a_params);
	}

	RE::BGSSoundDescriptorForm* Game_GetSound_DisabledAction()
	{
		using func_t = decltype(&Game_GetSound_DisabledAction);
		REL::Relocation<func_t> func{ REL::RelocationID(34315, 0) };  // 569620
		return func();
	}

	void BGSSoundDescriptorForm_Play(RE::BGSSoundDescriptorForm* sound)
	{
		using func_t = decltype(&BGSSoundDescriptorForm_Play);
		REL::Relocation<func_t> func{ REL::RelocationID(52055, 0) };  // 8da8d0
		return func(sound);
	}
}
