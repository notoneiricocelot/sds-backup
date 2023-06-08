#include <SKSE/SKSE.h>

#include "Scaleform.h"
#include "Settings.h"

#include "LevelUp.h"
#include "ClassMenu.h"

namespace SDS
{

	void SDSScaleform::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(SDSScaleform::GetSingleton());
		//SKSE::log::info("Registered {} event"sv, typeid(RE::MenuOpenCloseEvent).name());

		ui->Register(SDSClassMenu::Name(), SDSClassMenu::Create);
	}

	SDSScaleform::EventResult SDSScaleform::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (a_event && a_event->menuName == RE::RaceSexMenu::MENU_NAME) {
			if (!a_event->opening) {
				//auto ui = RE::UI::GetSingleton();

				if (Settings::bSSLDebugEnabled)
					SKSE::log::info("Sending message to SDSClassMenu"sv);

				// ui->GetEventSource<RE::MenuOpenCloseEvent>()->RemoveEventSink(SDSScaleform::GetSingleton());
				if (Settings::Saved->SelectedSpecializationID.length() == 0)
					SDSClassMenu::Open();
			}
		} else if (a_event && a_event->menuName == RE::LevelUpMenu::MENU_NAME) {
			LevelUpMenuHandler::OnMenuChange(a_event->opening);
		} else if (a_event && a_event->menuName == RE::DialogueMenu::MENU_NAME) {
			if (a_event->opening) {
			} else {
			}
		} else if (a_event && a_event->menuName == RE::TweenMenu::MENU_NAME) {
			if (a_event->opening) {
			} else {
			}
		}
		return EventResult::kContinue;
	}

	SDSScaleform::SDSScaleform() :
		_lock()
	{}
}
