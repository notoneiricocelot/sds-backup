#pragma once

#include "HandlerRaceSexMenu.h"

namespace SDS
{
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
				_RaceSexMenu_ProcessMessage = vTable3.write_vfunc(0x4, &RaceSexMenu_ProcessMessage);

				SKSE::Trampoline& trampoline = SKSE::GetTrampoline();

				REL::Relocation<uintptr_t> tweenMenuHook1{ RELOCATION_ID(51836, 0) };  // 8d0f80
				_TweenMenu_OpenHighlightedMenu = trampoline.write_branch<5>(tweenMenuHook1.address() + RELOCATION_OFFSET(0xE, 0x0), &TweenMenu_OpenHighlightedMenu);

				SDSHandlerRaceSexMenu::AddHooks();
			}

		public:
			static void LevelUpMenu_AcceptProcessorHook(RE::LevelUpMenu*, RE::FxDelegateHandler::CallbackProcessor*);
			static void LevelUpMenu_ConfirmLevelUpHook(RE::LevelUpMenu*, void*);

			static void RaceSexMenu_AcceptProcessorHook(RE::RaceSexMenu*, RE::FxDelegateHandler::CallbackProcessor*);
			static RE::UI_MESSAGE_RESULTS RaceSexMenu_ProcessMessage(RE::RaceSexMenu*, RE::UIMessage*);

			static inline REL::Relocation<decltype(LevelUpMenu_AcceptProcessorHook)> _LevelUpMenu_AcceptProcessorHook;
			static inline REL::Relocation<decltype(LevelUpMenu_ConfirmLevelUpHook)> _LevelUpMenu_ConfirmLevelUpHook;

			static inline REL::Relocation<decltype(RaceSexMenu_AcceptProcessorHook)> _RaceSexMenu_AcceptProcessorHook;
			static inline REL::Relocation<decltype(RaceSexMenu_ProcessMessage)> _RaceSexMenu_ProcessMessage;

			static void TweenMenu_OpenHighlightedMenu(RE::TweenMenu*, int);
			static inline REL::Relocation<decltype(TweenMenu_OpenHighlightedMenu)> _TweenMenu_OpenHighlightedMenu;
	};

}
