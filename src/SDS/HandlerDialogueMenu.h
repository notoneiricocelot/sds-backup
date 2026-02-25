#pragma once

namespace SDSHandlerDialogueMenu
{
	void OnMenuChange(bool);

	void AddHooks();

	void DialogueMenu_SetSpeaker(RE::DialogueMenu*, RE::BSUIMessageData*);
	inline REL::Relocation<decltype(DialogueMenu_SetSpeaker)> _DialogueMenu_SetSpeaker;

	RE::UI_MESSAGE_RESULTS DialogueMenu_ProcessMessage(RE::DialogueMenu*, RE::UIMessage*);
	inline REL::Relocation<decltype(DialogueMenu_ProcessMessage)> _DialogueMenu_ProcessMessage;
}
