#pragma once

namespace SDS
{

	class LevelUpMenuHookHandlers
	{
		public:
			static void CompleteLevelUp(const RE::FxDelegateArgs& a_params);
			static void CalculatePlayerStatsForAttribute(const RE::FxDelegateArgs& a_params);
			static void HighlightPlayerCard(const RE::FxDelegateArgs& a_params);

			static void AddStats();

	};

	class RaceSexMenuHookHandlers
	{
		public:
			static void ChangeClass(const RE::FxDelegateArgs& a_params);
			static void GetNewBonuses(const RE::FxDelegateArgs& a_params);
	};

	class HUDHooks
	{
		public:
			static void Hook()
			{
				// 1.5.97 only
				REL::Relocation<std::uintptr_t> vTable1(RE::VTABLE_LevelUpMenu[0]);
				REL::Relocation<std::uintptr_t> vTable2(RE::VTABLE___ConfirmLevelUpAttributeCallback[0]);
				REL::Relocation<std::uintptr_t> vTable3(RE::VTABLE_RaceSexMenu[0]);

				// Adding hook to LevelUpMenu->CallbackProcessor::Accept to inject some flash methods for swf api
				_LevelUpMenu_AcceptProcessorHook = vTable1.write_vfunc(0x1, &LevelUpMenu_AcceptProcessorHook);
				// Adding hook to ConfirmLevelUp when player press Yes button in confirm levelup dialog
				_LevelUpMenu_ConfirmLevelUpHook = vTable2.write_vfunc(0x1, &LevelUpMenu_ConfirmLevelUpHook);
				_RaceSexMenu_AcceptProcessorHook = vTable3.write_vfunc(0x1, &RaceSexMenu_AcceptProcessorHook);
			}

			static void PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* player, RE::ActorValue avID, uint8_t count);

			static void RaceSexMenu_ChangeRace(RE::FxDelegateArgs* callArgs);
			static void RaceSexMenu_ChangeSex(RE::FxDelegateArgs* callArgs);
			static void RaceSexMenu_ChangeName(RE::FxDelegateArgs* callArgs);

			//static void IMenu_ZoomPC(RE::IMenu *menu, bool inOut);
			static void IMenu_ZoomPC(const RE::FxDelegateArgs& a_params);

			static void MessageBoxMenu_PlaySound(const RE::FxDelegateArgs& a_params);
		private:
			static void LevelUpMenu_AcceptProcessorHook(RE::LevelUpMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor);
			static void LevelUpMenu_ConfirmLevelUpHook(RE::LevelUpMenu* a_this, void* a_data);
			static void RaceSexMenu_AcceptProcessorHook(RE::RaceSexMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor);

			static inline REL::Relocation<decltype(LevelUpMenu_AcceptProcessorHook)> _LevelUpMenu_AcceptProcessorHook;
			static inline REL::Relocation<decltype(LevelUpMenu_ConfirmLevelUpHook)> _LevelUpMenu_ConfirmLevelUpHook;
			static inline REL::Relocation<decltype(RaceSexMenu_AcceptProcessorHook)> _RaceSexMenu_AcceptProcessorHook;
	};

	class CoreHooks
	{
		public:
			static void Hook()
			{
				// _BSTRaceArray_Start = { REL::RelocationID(13541, 0) };  // 165660
				// _BSTRaceArray_Next = { REL::RelocationID(13546, 0) };   // 165710
				// FxResponseArgs_0_AddTesRace { REL::RelocationID(51570, 0) };  // 8BC140
			}

			static void FxResponseArgsList_Clear(RE::FxResponseArgsBase* self);

			static void* GfxMovieView_InvokeCallback(RE::GFxMovieView* self, const char* callPath, RE::FxResponseArgsBase* argsList);
	};

}
