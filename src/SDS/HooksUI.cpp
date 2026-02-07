#include "HooksUI.h"
#include "HooksCore.h"
#include "Settings.h"
#include "Specialization.h"

#include "HandlerLevelUpMenu.h"
#include "HandlerRaceSexMenu.h"

#include <typeinfo>

namespace SDS
{

	/**
		UI hooks
	*/
	void HUDHooks::LevelUpMenu_AcceptProcessorHook(RE::LevelUpMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		_LevelUpMenu_AcceptProcessorHook(a_this, a_processor);
		if (Settings::bDebugEnabled && Settings::iLogLevel > 2)
			SKSE::log::info("{}::Accept called", a_this->MENU_NAME);

		a_processor->Process("completeLevelUp", HandlerLevelUpMenu::CompleteLevelUp);
		a_processor->Process("calculatePlayerStatsForAttribute", HandlerLevelUpMenu::CalculatePlayerStatsForAttribute);
		a_processor->Process("highlightPlayerCard", HandlerLevelUpMenu::HighlightPlayerCard);
	}

	void HUDHooks::LevelUpMenu_ConfirmLevelUpHook(RE::LevelUpMenu* a_this, void* a_data)
	{
		HandlerLevelUpMenu::AddStats();
		_LevelUpMenu_ConfirmLevelUpHook(a_this, a_data);
	}

	void HUDHooks::RaceSexMenu_AcceptProcessorHook(RE::RaceSexMenu* a_this, RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		_RaceSexMenu_AcceptProcessorHook(a_this, a_processor);
		if (Settings::bDebugEnabled && Settings::iLogLevel > 2)
			SKSE::log::info("{}::Accept called", a_this->MENU_NAME);

		a_processor->Process("RemoveAllItems", SDSHandlerRaceSexMenu::RemoveAllItems);
		a_processor->Process("ChangeClass", SDSHandlerRaceSexMenu::ChangeClass);
		a_processor->Process("GetNewBonuses", SDSHandlerRaceSexMenu::GetNewBonuses);
	}

	RE::UI_MESSAGE_RESULTS HUDHooks::RaceSexMenu_ProcessMessage(RE::RaceSexMenu* a_this, RE::UIMessage* uiData)
	{
		// Disabling R (X on gamepad) button handling for RaceSexMenu to not show confirm menu popup
		if (uiData && uiData->type && (static_cast<int>(uiData->type.get()) - 6) == 1)
			return RE::UI_MESSAGE_RESULTS::kPassOn;
		return _RaceSexMenu_ProcessMessage(a_this, uiData);
	}

	void HUDHooks::TweenMenu_OpenHighlightedMenu(RE::TweenMenu* a_this, int selection)
	{
		_TweenMenu_OpenHighlightedMenu(a_this, selection);
		if (selection == 5)
		{
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TweenMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, 0);
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::JournalMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, 0);
		}
	}

}
