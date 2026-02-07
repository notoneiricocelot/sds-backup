#include <SKSE/SKSE.h>

#include "Scaleform.h"

#include "HooksCore.h"
#include "HandlerLevelUpMenu.h"
#include "HandlerRaceSexMenu.h"

namespace SDS
{

	void SDSScaleform::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(SDSScaleform::GetSingleton());
		SKSE::log::info("Registered {} event"sv, typeid(RE::MenuOpenCloseEvent).name());
	}

	SDSScaleform::EventResult SDSScaleform::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		//if (Settings::bSSLDebugEnabled && a_event && a_event->opening)
			// SKSE::log::info("{} is opening", a_event->menuName);

		if (a_event && a_event->menuName == RE::RaceSexMenu::MENU_NAME)
		{
			SDSHandlerRaceSexMenu::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::LevelUpMenu::MENU_NAME)
		{
			HandlerLevelUpMenu::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::DialogueMenu::MENU_NAME)
		{
		}
		else if (a_event && a_event->menuName == RE::TweenMenu::MENU_NAME)
		{
		}
		return EventResult::kContinue;
	}

	SDSScaleform::SDSScaleform() :
		_lock()
	{}

}
