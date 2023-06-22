#pragma once

namespace SDS
{

	namespace Offsets
	{

	}

	class HUDHooks
	{
	public:
		static void Hook()
		{
			REL::Relocation<std::uintptr_t> vTable1(RE::VTABLE_LevelUpMenu[0]);
			REL::Relocation<std::uintptr_t> vTable2(RE::VTABLE___ConfirmLevelUpAttributeCallback[0]);

			// Adding hook to LevelUpMenu->CallbackProcessor::Accept to inject some flash methods for swf api
			_LevelUpMenu_AcceptProcessorHook = vTable1.write_vfunc(0x1, &LevelUpMenu_AcceptProcessorHook);
			// Adding hook to ConfirmLevelUp when player press Yes button in confirm levelup dialog
			_LevelUpMenu_ConfirmLevelUpHook = vTable2.write_vfunc(0x1, &LevelUpMenu_ConfirmLevelUpHook);

			_PlayerCharacter_IncrementSkillHook = { RELOCATION_ID(39414, 0) }; // 6a2580 -> 1.5.97
		}

		static void PlayerCharacter_IncrementSkillHook(RE::TESObjectREFR* referer, uint32_t avID, uint32_t count);
		static inline REL::Relocation<decltype(PlayerCharacter_IncrementSkillHook)> _PlayerCharacter_IncrementSkillHook;
	private:
		static void LevelUpMenu_AcceptProcessorHook(RE::LevelUpMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor);
		static void LevelUpMenu_ConfirmLevelUpHook(RE::LevelUpMenu* a_this, void* a_data);

		static inline REL::Relocation<decltype(LevelUpMenu_AcceptProcessorHook)> _LevelUpMenu_AcceptProcessorHook;
		static inline REL::Relocation<decltype(LevelUpMenu_ConfirmLevelUpHook)> _LevelUpMenu_ConfirmLevelUpHook;
	};
}
