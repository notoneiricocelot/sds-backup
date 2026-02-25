#include <SKSE/SKSE.h>

#include "Scaleform.h"
#include "Settings.h"

#include "HooksCore.h"
#include "HandlerDialogueMenu.h"
#include "HandlerLevelUpMenu.h"
#include "HandlerRaceSexMenu.h"

#include "BonfireMenu.h"

namespace SDS
{

	void SDSScaleform::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(SDSScaleform::GetSingleton());
		if (Settings::bDebugEnabled && Settings::iLogLevel <= spdlog::level::level_enum::info)
			SKSE::log::info("Registered {} event"sv, typeid(RE::MenuOpenCloseEvent).name());

		RE::UI::GetSingleton()->Register(BonfireMenu::MENU_NAME, BonfireMenu::Creator);

	}

	RE::BSEventNotifyControl SDSScaleform::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		//if (Settings::bSSLDebugEnabled && a_event && a_event->opening)
			// SKSE::log::info("{} is opening", a_event->menuName);

		if (a_event && a_event->menuName == RE::RaceSexMenu::MENU_NAME)
		{
			SDSHandlerRaceSexMenu::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::LevelUpMenu::MENU_NAME)
		{
			SDSHandlerLevelUpMenu::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::DialogueMenu::MENU_NAME)
		{
			SDSHandlerDialogueMenu::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::TweenMenu::MENU_NAME)
		{
		}
		return RE::BSEventNotifyControl::kContinue;
	}

	SDSScaleform::SDSScaleform() :
		_lock()
	{}

}
