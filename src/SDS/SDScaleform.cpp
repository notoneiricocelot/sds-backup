#include <SKSE/SKSE.h>

#include "SDSScaleform.h"

namespace SDS
{
	// These four-character record types, which store data in the SKSE cosave, are little-endian. That means they are
	// reversed from the character order written here. Using the byteswap functions reverses them back to the order
	// the characters are written in the source code.

	/*

	*/
	void SDSScaleform::OnRevert(SKSE::SerializationInterface*)
	{
		Locker locker(GetSingleton()->_lock);
	}

	void SDSScaleform::OnGameLoaded([[maybe_unused]] SKSE::SerializationInterface* serde)
	{
		[[maybe_unused]] std::uint32_t type = 0;
		[[maybe_unused]] std::uint32_t size = 0;
		[[maybe_unused]] std::uint32_t version = 0;
		// To load records from a cosave, use <code>GetNextRecordInfo</code> to iterate from one record to the next.
		// You will be given records in the order they were written, but otherwise you do not look up a record by its name.
		// Instead check the result of each iteration to find out which record it is and handle it appropriately.
		//
		// If you make breaking changes to your data format, you can increase the version number used when writing the data
		// out and check that number here to handle previous versions.
		// while (serde->GetNextRecordInfo(type, version, size)) {
		// }
	}

	void SDSScaleform::OnGameSaved(SKSE::SerializationInterface* serde)
	{
		Locker locker(GetSingleton()->_lock);
		(void)serde;
		// To write data open a record with a given name. The name must be unique within your mod, and has a version number
		// assigned (in this case 0). You can increase the version number if making breaking format change from a previous
		// version of your mod, so that the load handler can know which version of the format is being used.
	}

	void SDSScaleform::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(SDSScaleform::GetSingleton());
		//SKSE::log::info("Registered {} event"sv, typeid(RE::MenuOpenCloseEvent).name());

		ui->Register(SDSClassMenu::Name(), SDSClassMenu::Create);

		//auto scaleformProxy = SKSE::GetScaleformInterface();
		//scaleformProxy->Register(SDSScaleform::RegisterScaleform, Plugin::NAME.c_str());
	}

	bool SDSScaleform::RegisterScaleform([[maybe_unused]] RE::GFxMovieView* a_view, [[maybe_unused]] RE::GFxValue* a_root)
	{
		const char* currentSWFPathString = a_view->GetMovieDef()->GetFileURL();

		if (strcmp(currentSWFPathString, SDSClassMenu::FILE_PATH)) {
			SKSE::log::info("Something recieved: {}"sv, currentSWFPathString);
		}
		//SKSE::RegisterFunction<SDSSelectClass>(a_view, a_root, "SDSSelectClass");
		return true;
	}

	SDSScaleform::EventResult SDSScaleform::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (a_event && a_event->menuName == RE::RaceSexMenu::MENU_NAME) {
			if (!a_event->opening) {
				auto ui = RE::UI::GetSingleton();

				//SKSE::log::info("Sending message to SDSClassMenu"sv);
				ui->GetEventSource<RE::MenuOpenCloseEvent>()->RemoveEventSink(SDSScaleform::GetSingleton());
				SDSClassMenu::Open();
			}
		} else if (a_event && a_event->menuName == RE::LevelUpMenu::MENU_NAME) {
			if (a_event->opening) {
			} else {
			}
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
