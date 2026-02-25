#pragma once

namespace SDSHandlerLevelUpMenu
{
	void ReloadUI();
	void AddStats();

	class ConfirmLevelUpMessageCallback : public RE::IMessageBoxCallback
	{
	public:
		ConfirmLevelUpMessageCallback();

		virtual ~ConfirmLevelUpMessageCallback() = default;

		virtual void Run(RE::IMessageBoxCallback::Message) override;
	};

	void OnMenuChange(bool isOpening);

	void CompleteLevelUp(const RE::FxDelegateArgs&);
	void CalculatePlayerStatsForAttribute(const RE::FxDelegateArgs&);
	void HighlightPlayerCard(const RE::FxDelegateArgs&);
	void CloseMenu(const RE::FxDelegateArgs&);

	void AddHooks();

	bool StatsMenu_IsLevelUpAvailable(RE::PlayerCharacter::PlayerSkills*);
	inline REL::Relocation<decltype(StatsMenu_IsLevelUpAvailable)> _StatsMenu_IsLevelUpAvailable;
}
