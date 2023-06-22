#pragma once

#include "Specialization.h"

namespace SDS
{

	class SDSClassMenu : public RE::IMenu
	{
		public:
			using GRefCountBaseStatImpl::operator new;
			using GRefCountBaseStatImpl::operator delete;

			SDSClassMenu();
			virtual ~SDSClassMenu() = default;

			//IMenu
			virtual void Accept(RE::FxDelegateHandler::CallbackProcessor* a_processor) override;
			virtual RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override;

			static void Open();
			static void Close();

			static RE::IMenu* Create();

			static constexpr std::string_view Name();
			constexpr static const char* FILE_PATH = "Interface/SDSClassSelectionMenu.swf";
			constexpr static SDSMenuType MENU_TYPE = SDSMenuType::kClassMenu;

		private:
			void OnMenuOpen();
			void OnMenuClose();

			static void SDSClassSelected(const RE::FxDelegateArgs& a_params);
			static void SDSClassSelected(std::string a_id);

			void InitExtensions();
			virtual void AddToClassList(Specialization* spec, RE::GFxValue& a_array);

			RE::GFxValue _compas_alpha;

	};

	constexpr std::string_view SDSClassMenu::Name()
	{
		return "SDSClassSelectionMenu";
	}

}
