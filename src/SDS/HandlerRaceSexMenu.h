#pragma once

namespace SDSHandlerRaceSexMenu
{
	void RemoveAllPlayerItems(bool);
	void UpdateCameraPosition();

	void OnMenuChange(bool);

	void ChangeClass(const RE::FxDelegateArgs&);
	void GetNewBonuses(const RE::FxDelegateArgs&);
	void RemoveAllItems(const RE::FxDelegateArgs&);
	void UpdateCameraPositionCallback(const RE::FxDelegateArgs&);
	void ToggleEquipmentSounds(bool);

	void AddHooks();

	void UpdateCamera_ShowHook(RE::RaceSexMenu*);
	inline REL::Relocation<decltype(UpdateCamera_ShowHook)> _UpdateCamera_ShowHook;

	void UpdateCamera_AdvanceMovieHook(RE::RaceSexMenu*);
	inline REL::Relocation<decltype(UpdateCamera_AdvanceMovieHook)> _UpdateCamera_AdvanceMovieHook;

	void MessageBox_CloseMenuHook(RE::RaceSexMenu*);
	inline REL::Relocation<decltype(MessageBox_CloseMenuHook)> _MessageBox_CloseMenuHook;
}
