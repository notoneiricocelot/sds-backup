#include "HandlerRaceSexMenu.h"

#include "Specialization.h"
#include "Settings.h"

#include "HooksCore.h"

namespace SDSHandlerRaceSexMenu
{
	void RemoveAllPlayerItems(bool doUpdate3D)
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		RE::InventoryChanges* playerChanges = player->GetInventoryChanges();

		if (playerChanges) {
			playerChanges->RemoveAllItems(player->AsReference(), nullptr, 0, 0, 0);
			SDSHooksCore::PlayerCharacter_RemoveAllItems(player, player);
		}

		if (doUpdate3D)
			player->Update3DModel();
	}

	void UpdateCameraPosition()
	{
		REL::Relocation<uintptr_t> RaceSexCamera_ZoomOutPos{ RELOCATION_ID(519854, 0) };  // 2f4e798
		REL::Relocation<uintptr_t> RaceSexCamera_ZoomInPos{ RELOCATION_ID(519849, 0) };   // 2f4e778
		RE::NiQuaternion* pos1 = (RE::NiQuaternion*)RaceSexCamera_ZoomOutPos.address();
		RE::NiQuaternion* pos2 = (RE::NiQuaternion*)RaceSexCamera_ZoomInPos.address();

		pos1->w += 9;
		pos2->w += 20;
	}

	void OnMenuChange(bool isOpening)
	{
		if (isOpening)
		{
			if (SDS::Settings::Saved->SelectedSpecializationID.length() > 0)
				return;

			ToggleEquipmentSounds(true);

			RE::GPtr<RE::GFxMovieView> raceMenuMovie = RE::UI::GetSingleton()->GetMenu(RE::RaceSexMenu::MENU_NAME).get()->uiMovie;

			RE::FxResponseArgs<1> classData;
			RE::GFxValue classArray;
			raceMenuMovie->CreateArray(&classArray);

			for (auto it = SDS::Specialization::Specializations.begin(); it != SDS::Specialization::Specializations.end(); it++)
			{
				RE::GFxValue data;
				raceMenuMovie->CreateObject(&data);

				it->get()->ToGfxValue(&data, raceMenuMovie);
				classArray.PushBack(data);
			}
			classData.Add(classArray);

			SDSHooksCore::GfxMovieView_InvokeCallback(raceMenuMovie.get(), "SetClassList", &classData);

			if (SDS::Settings::bDebugEnabled && SDS::Settings::iLogLevel > 3)
				SKSE::log::info("Menu {} loaded, {} classes added"sv, RE::RaceSexMenu::MENU_NAME, classArray.GetArraySize());
		}
		else
		{
			ToggleEquipmentSounds(false);
		}
	}

	void ChangeClass(const RE::FxDelegateArgs& a_params)
	{
		if (a_params.GetArgCount() != 1)
			return;

		const char* a_id = a_params[0].GetString();
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		// setup initial attribute values
		for (auto it = SDS::Settings::LevelingSettings.begin(); it < SDS::Settings::LevelingSettings.end(); it++)
		{
			it->SetBaseActorValue(player, 0.0f + SDS::Settings::iBaseAttributeValue);
		}

		SDS::Settings::Saved->SelectedSpecializationID = std::string(a_id);
		if (SDS::Settings::bDebugEnabled)
			SKSE::log::info("Class {} selected"sv, a_id);
	}

	/**
	* @param raceID char*
	* @param specID char*
	* @param doChangeEquipment bool
	*/
	void GetNewBonuses(const RE::FxDelegateArgs& a_params)
	{
		if (a_params.GetArgCount() < 3)
			return;

		RE::RaceSexMenu* self = static_cast<RE::RaceSexMenu*>(a_params.GetHandler());
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

		const int raceID = static_cast<int>(a_params[0].GetUInt());
		const char* specID = a_params[1].GetString();

		if (SDS::Settings::bDebugEnabled && SDS::Settings::iLogLevel > 3)
			SKSE::log::info("Updating ui bonuses for spec {} and race {}"sv, specID, raceID);

		int selectedSpecIndex = SDS::Specialization::Find(specID);

		// unk140 - various body and face morphs preloaded by the game for character customization
		// used by racesexmenu with step 5 for every race and sex
		RE::TESRace* playerRace = ((RE::TESRace* ) self->unk140[self->sex.get()][5 * raceID]);
		RE::FormID playerRaceID = playerRace->formID;

		RE::FxResponseArgs<8 + 18> callParams;

		SDS::SDSAttribute firstAttribute = SDS::Specialization::Specializations[selectedSpecIndex]->GetFirstAttribute();
		SDS::SDSAttribute secondAttribute = SDS::Specialization::Specializations[selectedSpecIndex]->GetSecondAttribute();
		int bonusResult = 0;

		for (auto it = SDS::Settings::LevelingSettings.begin(); it < SDS::Settings::LevelingSettings.end(); it++)
		{
			bonusResult = it->racialBonuses[playerRaceID] + SDS::Settings::iBaseAttributeValue;
			if (it->attribute == firstAttribute || it->attribute == secondAttribute)
				bonusResult = bonusResult + SDS::Settings::iClassBaseAttributeBonus;
			callParams.Add(RE::GFxValue(bonusResult));
		}

		std::array<int, 18> skillBoosts{};
		int i = 0;
		int avIndex = 0;

		for (i = 0; i < 6; i++)
		{
			avIndex = SDS::Specialization::Specializations[selectedSpecIndex]->skills[i] - 6;
			skillBoosts[avIndex] = SDS::Settings::iClassBaseSkillBonus;
		}

		for (i = 0; i < RE::RACE_DATA::kNumSkillBoosts; i++)
		{
			if (player->GetActorBase()->race->data.skillBoosts[i].skill.get() == RE::ActorValue::kNone)
				continue;

			avIndex = static_cast<int>(player->GetActorBase()->race->data.skillBoosts[i].skill.get()) - 6;
			skillBoosts[avIndex] = skillBoosts[avIndex] + player->GetActorBase()->race->data.skillBoosts[i].bonus;
		}

		for (i = 0; i < 18; i++)
		{
			callParams.Add(RE::GFxValue(skillBoosts[i]));
		}

		SDSHooksCore::GfxMovieView_InvokeCallback(self->uiMovie.get(), "UpdateBonusesInfo", &callParams);

		bool doUpdateEquipment = a_params[2].GetBool();
		if (doUpdateEquipment)
		{
			RemoveAllPlayerItems(false);

			RE::TESLeveledList* loadout = SDS::GetTESFormFromSetting<RE::TESLevItem>(SDS::Specialization::Specializations[selectedSpecIndex]->equipment);
			RE::TESForm* levItem;

			for (i = 0; i < loadout->numEntries; i++)
			{
				levItem = loadout->entries[i].form;

				switch (levItem->formType.get())
				{
					case RE::FormType::Armor:
						[[fallthrough]];
					case RE::FormType::Weapon:
						player->AsReference()->AddObjectToContainer(levItem->As<RE::TESBoundObject>(), nullptr, loadout->entries[i].count, player->AsReference());
						RE::ActorEquipManager::GetSingleton()->EquipObject(player, levItem->As<RE::TESBoundObject>(), nullptr, loadout->entries[i].count, nullptr, true, false, false, false);
						break;
					case RE::FormType::Spell:
						// player->AddSpell(levItem->As<RE::SpellItem>());
						break;
					case RE::FormType::Shout:
						// player->AddShout(levItem->As<RE::TESShout>());
						break;
					case RE::FormType::Scroll:
						[[fallthrough]];
					case RE::FormType::Misc:
						[[fallthrough]];
					default:
						player->AsReference()->AddObjectToContainer(levItem->As<RE::TESBoundObject>(), nullptr, loadout->entries[i].count, player->AsReference());
						break;
				}
			}

			player->Update3DModel();
		}
	}

	void RemoveAllItems(const RE::FxDelegateArgs&)
	{
		RemoveAllPlayerItems(true);
	}

	void UpdateCameraPositionCallback(const RE::FxDelegateArgs&)
	{
		UpdateCameraPosition();
	}

	void ToggleEquipmentSounds(bool doMute)
	{
		// TODO
		// 1 - AudioCategorySFXFadeDuringDialogue
		// 3 - AudioCategoryPausedDuringMenuLoad
		RE::BGSSoundCategory* cat1 = RE::TESDataHandler::GetSingleton()->formArrays[130][1]->As<RE::BGSSoundCategory>();
		RE::BGSSoundCategory* cat2 = RE::TESDataHandler::GetSingleton()->formArrays[130][3]->As<RE::BGSSoundCategory>();

		if (doMute)
		{
			cat1->SetCategoryVolume(0);
			cat2->SetCategoryVolume(0);
		}
		else
		{
			cat1->SetCategoryVolume(cat1->GetDefaultMenuValue());
			cat2->SetCategoryVolume(cat2->GetDefaultMenuValue());
		}
	}

	void AddHooks()
	{
		REL::Relocation<uintptr_t> raceSexCamera_Hook1{ RELOCATION_ID(51482, 0) };  // 8AFF40 + 143/call
		REL::Relocation<uintptr_t> raceSexCamera_Hook2{ RELOCATION_ID(51484, 0) };  // 8B0A60 + 132/call
		REL::Relocation<uintptr_t> msgBoxConfirm_Hook3{ RELOCATION_ID(51540, 0) };  // 8BA590 + 168/call

		SKSE::Trampoline& trampoline = SKSE::GetTrampoline();
		_UpdateCamera_ShowHook = trampoline.write_call<5>(raceSexCamera_Hook1.address() + RELOCATION_OFFSET(0x143, 0x0), &UpdateCamera_ShowHook);
		_UpdateCamera_AdvanceMovieHook = trampoline.write_call<5>(raceSexCamera_Hook2.address() + RELOCATION_OFFSET(0x132, 0x0), &UpdateCamera_AdvanceMovieHook);

		_MessageBox_CloseMenuHook = trampoline.write_call<5>(msgBoxConfirm_Hook3.address() + RELOCATION_OFFSET(0x168, 0x0), &MessageBox_CloseMenuHook);
	}

	void UpdateCamera_ShowHook(RE::RaceSexMenu* a_this)
	{
		_UpdateCamera_ShowHook(a_this);
		UpdateCameraPosition();
	}

	void UpdateCamera_AdvanceMovieHook(RE::RaceSexMenu* a_this)
	{
		_UpdateCamera_AdvanceMovieHook(a_this);
		UpdateCameraPosition();
	}

	void MessageBox_CloseMenuHook(RE::RaceSexMenu* a_this)
	{
		_MessageBox_CloseMenuHook(a_this);

		int i = 0;
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	
		for (; i < SDS::Specialization::Specialization::SKILLS_MAX; i++)
		{
			if (SDS::Specialization::Specializations[SDS::Settings::selectedSpecIndex]->skills[i] > 0)
				SDSHooksCore::PlayerCharacter_IncrementSkillHook(player, static_cast<RE::ActorValue>(SDS::Specialization::Specializations[SDS::Settings::selectedSpecIndex]->skills[i]), SDS::Settings::iClassBaseSkillBonus);
		}

		for (i = 0; i < SDS::SDSAttribute::kFaith; i++)
		{
			SDS::Settings::LevelingSettings[i].SetBaseActorValue(player, SDS::Settings::iBaseAttributeValue + 0.0f);
		}

		SDS::SDSAttribute attr1 = SDS::Specialization::Specializations[SDS::Settings::selectedSpecIndex]->GetFirstAttribute();
		SDS::SDSAttribute attr2 = SDS::Specialization::Specializations[SDS::Settings::selectedSpecIndex]->GetSecondAttribute();

		SDS::Settings::LevelingSettings[attr1 - 1].IncrementAttribute(player, static_cast<float>(SDS::Settings::iClassBaseAttributeBonus));
		SDS::Settings::LevelingSettings[attr2 - 1].IncrementAttribute(player, static_cast<float>(SDS::Settings::iClassBaseAttributeBonus));
	}
}
