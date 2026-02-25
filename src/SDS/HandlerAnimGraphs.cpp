#include "HandlerAnimGraphs.h"

#include "BonfireMenu.h"
#include "Common.h"

namespace SDS
{
	BonfireAnimationGraphManager* BonfireAnimationGraphManager::GetSingleton()
	{
		static BonfireAnimationGraphManager animSink;
		return &animSink;
	}

	RE::BSEventNotifyControl BonfireAnimationGraphManager::ProcessEvent(const RE::BSAnimationGraphEvent* graphEvent, RE::BSTEventSource<RE::BSAnimationGraphEvent>*)
	{
		// SKSE::log::info("event data: [{}, {}, {}]", graphEvent->holder->GetFormEditorID(), graphEvent->payload.data(), graphEvent->tag.data());
		if (graphEvent->tag == SDSBonfireSitAnimationEnd)
		{
			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			player->RemoveAnimationGraphEventSink(BonfireAnimationGraphManager::GetSingleton());

			RE::UIMessageQueue::GetSingleton()->AddMessage(BonfireMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
		}
		return RE::BSEventNotifyControl::kContinue;
	}
}
