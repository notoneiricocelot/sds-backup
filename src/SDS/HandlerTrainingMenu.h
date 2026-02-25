#pragma once

namespace SDSHandlerTrainingMenu
{
	void UpdatePlayerCard(RE::TrainingMenu*);
	void TrainFocus(const RE::FxDelegateArgs&);
	void AddHooks();

	void TrainingMenu_UpdateCardText(RE::TrainingMenu*);

	void TrainingMenu_UpdateText_Train(RE::TrainingMenu*);
	inline REL::Relocation<decltype(TrainingMenu_UpdateText_Train)> _TrainingMenu_UpdateText_Train;

	void TrainingMenu_UpdateText_UpdateCard(RE::TrainingMenu*);
	inline REL::Relocation<decltype(TrainingMenu_UpdateText_UpdateCard)> _TrainingMenu_UpdateText_UpdateCard;

	void TrainingMenu_Accept(RE::TrainingMenu*, RE::FxDelegateHandler::CallbackProcessor*);
	inline REL::Relocation<decltype(TrainingMenu_Accept)> _TrainingMenu_Accept;

	RE::UI_MESSAGE_RESULTS TrainingMenu_ProcessMessage(RE::TrainingMenu*, RE::UIMessage*);
	inline REL::Relocation<decltype(TrainingMenu_ProcessMessage)> _TrainingMenu_ProcessMessage;
}
