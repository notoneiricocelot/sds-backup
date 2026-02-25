#include "BonfireMenu.h"

#include "HooksCore.h"

#include "Common.h"
#include "Settings.h"

namespace SDS
{

	namespace BonfireMenuHandlers
	{
		void Leave(const RE::FxDelegateArgs&)
		{
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
			RE::UIMessageQueue::GetSingleton()->AddMessage(BonfireMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);

			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			// player->NotifyAnimationGraph(SDSBonfireSitAnimationRecovery);
			player->NotifyAnimationGraph(SDSAnimDefaultState);
		}

		void OpenTravelMenu(const RE::FxDelegateArgs&)
		{
			if (SDS::Settings::bTravelMenuEnabled)
			{

			}
			else
			{
				RE::GFxMovieView* dialogueMovie = RE::UI::GetSingleton()->GetMenu(RE::DialogueMenu::MENU_NAME)->uiMovie.get();
				RE::GFxValue _alpha;
				dialogueMovie->GetVariable(&_alpha, "_root.DialogueMenu_mc._alpha");
				_alpha.SetNumber(100.00);
				dialogueMovie->SetVariable("_root.DialogueMenu_mc._alpha", _alpha);

				RE::UIMessageQueue::GetSingleton()->AddMessage(BonfireMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
				RE::PlayerCharacter::GetSingleton()->NotifyAnimationGraph(SDSAnimDefaultState);
			}
		}
	}

	void BonfireMenu::Accept(RE::FxDelegateHandler::CallbackProcessor* cbProcessor)
	{
		cbProcessor->Process("OpenTravelMenu", BonfireMenuHandlers::OpenTravelMenu);
		cbProcessor->Process("Leave", BonfireMenuHandlers::Leave);
		cbProcessor->Process("PlayMenuSound", SDSHooksCore::MessageBoxMenu_PlaySound);
	}

	RE::UI_MESSAGE_RESULTS SDS::BonfireMenu::ProcessMessage(RE::UIMessage& msg)
	{
		switch (*msg.type)
		{
			case RE::UI_MESSAGE_TYPE::kShow:
				OnOpen();
				return RE::IMenu::ProcessMessage(msg);
			case RE::UI_MESSAGE_TYPE::kHide:
				OnClose();
				return RE::IMenu::ProcessMessage(msg);
			default:
				return RE::IMenu::ProcessMessage(msg);
		}
	}

	void BonfireMenu::OnOpen()
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		RE::FxResponseArgs<1> callArgs;
		// GFxValue cant store links to text data and require complete new values in certain situations
		callArgs.Add(RE::GFxValue(std::string(player->currentLocation->GetFullName())));

		SDSHooksCore::GfxMovieView_InvokeCallback(this->uiMovie.get(), "ChangeLocation", &callArgs);
		SDSHooksCore::GfxMovieView_InvokeCallback(this->uiMovie.get(), "StartOpenMenuAnim", &callArgs);

		SDS::Settings::Saved->bBonfireOpened = true;
	}

	void BonfireMenu::OnClose()
	{
		RE::FxResponseArgs<0> callArgs;
		SDSHooksCore::GfxMovieView_InvokeCallback(this->uiMovie.get(), "StartCloseMenuAnim", &callArgs);

		SDS::Settings::Saved->bBonfireOpened = false;
	}
}
