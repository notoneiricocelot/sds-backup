#include "SDSClassMenu.h"
#include "Settings.h"
#include "Papyrus.h"

namespace SDS
{
	// https://github.com/ahzaab/Skywind/blob/master/src/BirthSignMenu.cpp
	// https://github.com/ahzaab/Skywind/blob/master/src/SpellmakingMenu.cpp
	SDSClassMenu::SDSClassMenu()
	{
		auto loader = RE::BSScaleformManager::GetSingleton();
		[[maybe_unused]] auto success = loader->LoadMovieEx(this, SDSClassMenu::Name(), [this](RE::GFxMovieDef* a_def)
		{
			fxDelegate.reset(new RE::FxDelegate());
			fxDelegate->RegisterHandler(this);
			a_def->SetState(RE::GFxState::StateType::kExternalInterface, fxDelegate.get());
			fxDelegate->Release();

			a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<RE::GFxLog>().get());
		});

		depthPriority = 5;  // JournalMenu == 5
		menuFlags.set(RE::UI_MENU_FLAGS::kUpdateUsesCursor, RE::UI_MENU_FLAGS::kUsesCursor, RE::UI_MENU_FLAGS::kTopmostRenderedMenu,
			RE::UI_MENU_FLAGS::kUsesBlurredBackground, RE::UI_MENU_FLAGS::kDisablePauseMenu, RE::UI_MENU_FLAGS::kPausesGame);
		inputContext = RE::UserEvents::INPUT_CONTEXT_ID::kFavor;
		InitExtensions();
		uiMovie->SetVisible(false);
	}

	RE::UI_MESSAGE_RESULTS SDSClassMenu::ProcessMessage(RE::UIMessage& a_message)
	{
		switch (a_message.type.get()) {
		case RE::UI_MESSAGE_TYPE::kShow:
			OnMenuOpen();
			return RE::UI_MESSAGE_RESULTS::kHandled;
		case RE::UI_MESSAGE_TYPE::kHide:
			OnMenuClose();
			return RE::UI_MESSAGE_RESULTS::kHandled;
		default:
			return RE::IMenu::ProcessMessage(a_message);
		}
	}

	void SDSClassMenu::Open()
	{
		auto uiQueue = RE::UIMessageQueue::GetSingleton();
		uiQueue->AddMessage(SDSClassMenu::Name(), RE::UI_MESSAGE_TYPE::kShow, 0);
	}

	void SDSClassMenu::Close()
	{
		auto uiQueue = RE::UIMessageQueue::GetSingleton();
		uiQueue->AddMessage(SDSClassMenu::Name(), RE::UI_MESSAGE_TYPE::kHide, 0);
	}

	RE::IMenu* SDSClassMenu::Create()
	{
		return new SDSClassMenu();
	}

	void SDSClassMenu::AddToClassList(Specialization* spec, RE::GFxValue& a_array)
	{
		RE::GFxValue data;
		uiMovie->CreateObject(&data);

		RE::GFxValue classID;
		classID.SetString(spec->GetID());
		RE::GFxValue name;
		name.SetString(spec->GetName());
		RE::GFxValue desc;
		desc.SetString(spec->GetDescription());

		RE::GFxValue focus;
		focus.SetNumber(static_cast<int>(spec->GetFocus()));
		RE::GFxValue attr1;
		attr1.SetNumber(static_cast<int>(spec->GetFirstAttribute()));
		RE::GFxValue attr2;
		attr2.SetNumber(static_cast<int>(spec->GetSecondAttribute()));

		int* specSkills = spec->GetSkills();
		RE::GFxValue gfxSkills;
		uiMovie->CreateArray(&gfxSkills);

		for (int i = 0; i < 6; i++) {
			RE::GFxValue skill;
			skill.SetNumber(*(specSkills + i));
			gfxSkills.PushBack(skill);
		}

		data.SetMember("id", classID);
		data.SetMember("name", name);
		data.SetMember("description", desc);
		data.SetMember("focus", focus);
		data.SetMember("attr1", attr1);
		data.SetMember("attr2", attr2);
		data.SetMember("skills", gfxSkills);

		a_array.PushBack(data);
	}

	void SDSClassMenu::OnMenuOpen()
	{
		uiMovie->SetVisible(true);

		RE::GFxValue classArray;
		RE::GFxValue playerObject;
		RE::GFxValue menuCall;

		uiMovie->CreateArray(&classArray);
		uiMovie->CreateObject(&playerObject);

		for (int i = 0; i < Specialization::Specializations.size(); ++i) {
			AddToClassList(Specialization::Specializations[i].get(), classArray);
		}

		auto hud = RE::UI::GetSingleton()->GetMenu(RE::HUDMenu::MENU_NAME);
		hud.get()->uiMovie->GetVariable(&_compas_alpha, "HUDMovieBaseInstance.CompassShoutMeterHolder._alpha");

		RE::GFxValue zeroAlpha;
		zeroAlpha.SetNumber(0.f);
		hud.get()->uiMovie->SetVariable("HUDMovieBaseInstance.CompassShoutMeterHolder._alpha", zeroAlpha);

		uiMovie->Invoke("_root.ClassSelector_mc.loadClasses", nullptr, &classArray, 1);

		SKSE::log::info("Menu {} loaded, classes invoked, updating player data"sv, SDSClassMenu::Name());

		ScaleformHelper::FillPlayerObject(&playerObject);
		uiMovie->Invoke("_root.ClassSelector_mc.UpdatePlayerInfo", nullptr, &playerObject, 1);
	}

	void SDSClassMenu::OnMenuClose()
	{
		auto hud = RE::UI::GetSingleton()->GetMenu(RE::HUDMenu::MENU_NAME);
		hud.get()->uiMovie->SetVariable("HUDMovieBaseInstance.CompassShoutMeterHolder._alpha", _compas_alpha);
	}

	void SDSClassMenu::InitExtensions()
	{
		RE::GFxValue boolean(true);
		uiMovie->SetVariable("_global.gfxExtensions", boolean);
	}

	void SDSClassMenu::Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor)
	{
		a_processor->Process("SDSClassSelected", SDSClassSelected);
	}

	void SDSClassMenu::SDSClassSelected(const RE::FxDelegateArgs& a_params)
	{
		assert(a_params.GetArgCount() == 1);
		auto menu = static_cast<SDSClassMenu*>(a_params.GetHandler());
		auto a_id = a_params[0].GetString();
		menu->SDSClassSelected(a_id);
	}

	void SDSClassMenu::SDSClassSelected(std::string a_id)
	{
		SKSE::log::info("{} class selected"sv, a_id);
	}
}