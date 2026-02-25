#include "HandlerDialogueMenu.h"

// #include "BonfireMenu.h"
#include "Settings.h"
#include "HooksCore.h"
#include "HandlerAnimGraphs.h"

namespace SDSHandlerDialogueMenu
{
	void OnMenuChange(bool isOpening)
	{
		if (isOpening)
		{

		}
		else
		{

		}
	}

	void AddHooks()
	{
		REL::Relocation<uintptr_t> dialogueMenuProcessMessage_hook{ RELOCATION_ID(50612, 0) };  // 875D30 + 18B/call

		SKSE::Trampoline& trampoline = SKSE::GetTrampoline();
		_DialogueMenu_SetSpeaker = trampoline.write_call<5>(dialogueMenuProcessMessage_hook.address() + RELOCATION_OFFSET(0x18B, 0x0), &DialogueMenu_SetSpeaker);

		REL::Relocation<uintptr_t> vtable(RE::VTABLE_DialogueMenu[0]);
		_DialogueMenu_ProcessMessage = vtable.write_vfunc(0x4, &DialogueMenu_ProcessMessage);
	}

	void DialogueMenu_SetSpeaker(RE::DialogueMenu* aSelf, RE::BSUIMessageData* msgData)
	{
		_DialogueMenu_SetSpeaker(aSelf, msgData);

		RE::MenuTopicManager* topicManager = RE::MenuTopicManager::GetSingleton();

		RE::TESObjectREFR* speaker = topicManager->speaker.get().get();
		if (speaker->HasKeyword(SDS::Settings::Saved->bonfireKW))
		{
			RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
			player->AddAnimationGraphEventSink(SDS::BonfireAnimationGraphManager::GetSingleton());
			player->NotifyAnimationGraph(SDSBonfireSitAnimationStart);

			RE::GFxValue _alpha;
			aSelf->uiMovie->GetVariable(&_alpha, "_root.DialogueMenu_mc._alpha");
			_alpha.SetNumber(0.00);
			aSelf->uiMovie->SetVariable("_root.DialogueMenu_mc._alpha", _alpha);
			// return RE::UIMessageQueue::GetSingleton()->AddMessage(SDS::BonfireMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
		}

	}

	RE::UI_MESSAGE_RESULTS DialogueMenu_ProcessMessage(RE::DialogueMenu* aSelf, RE::UIMessage* aMsg)
	{
		if (!SDS::Settings::Saved->bBonfireOpened)
			return _DialogueMenu_ProcessMessage(aSelf, aMsg);
		else
			return RE::UI_MESSAGE_RESULTS::kPassOn;
	}

}
