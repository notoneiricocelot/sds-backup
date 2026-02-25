#pragma once
namespace SDS
{

	namespace BonfireMenuHandlers
	{
		void Leave(const RE::FxDelegateArgs&);
		void OpenTravelMenu(const RE::FxDelegateArgs&);
	}

	class BonfireMenu : RE::IMenu
	{

		void OnOpen();
		void OnClose();

	public:
		BonfireMenu()
		{
			RE::BSScaleformManager* loader = RE::BSScaleformManager::GetSingleton();
			loader->LoadMovie(this, this->uiMovie, MENU_NAME.data());

			depthPriority = 3;
			menuFlags.set(RE::UI_MENU_FLAGS::kUsesCursor, RE::UI_MENU_FLAGS::kTopmostRenderedMenu,
				RE::UI_MENU_FLAGS::kDontHideCursorWhenTopmost, RE::UI_MENU_FLAGS::kHasButtonBar, RE::UI_MENU_FLAGS::kUsesBlurredBackground);
			inputContext = RE::IMenu::Context::kJournal;
		}

		~BonfireMenu() = default;

		constexpr static std::string_view MENU_NAME = "Bonfire";

		static RE::stl::owner<RE::IMenu*> Creator() { return new BonfireMenu(); }

		void Accept(RE::FxDelegateHandler::CallbackProcessor*) override;
		RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage&) override;
	};

}
