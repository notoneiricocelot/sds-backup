#include "Hooks.h"
#include "LevelUp.h"

#include <typeinfo>

namespace SDS
{
	// public
	void HUDHooks::PlayerCharacter_IncrementSkillHook([[maybe_unused]] RE::TESObjectREFR* referer, uint32_t avID, uint32_t count)
	{
		_PlayerCharacter_IncrementSkillHook(referer, avID, count);
	}

	// private
	void HUDHooks::LevelUpMenu_AcceptProcessorHook(RE::LevelUpMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		_LevelUpMenu_AcceptProcessorHook(a_this, a_processor);
		a_processor->Process(LevelUpMenuHandler::CompleteLevelUp_Name, LevelUpMenuHandler::CompleteLevelUp);
	}

	void HUDHooks::LevelUpMenu_ConfirmLevelUpHook(RE::LevelUpMenu* a_this, void* a_data)
	{
		LevelUpMenuHandler::AddStats();
		_LevelUpMenu_ConfirmLevelUpHook(a_this, a_data);
	}

}
