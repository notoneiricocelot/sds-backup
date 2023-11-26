#include <SKSE/SKSE.h>

#include "Scaleform.h"
#include "ScaleformHelper.h"
#include "Specialization.h"
#include "Settings.h"

#include "Hooks.h"

namespace SDS
{

	void LevelUpMenuHandler::OnMenuChange(bool isOpening)
	{
		RE::ActorHandle playerHandle = RE::PlayerCharacter::GetSingleton()->GetHandle();

		if (isOpening) {
			RE::GPtr<RE::GFxMovieView> uiMovie = RE::UI::GetSingleton()->GetMenu(RE::LevelUpMenu::MENU_NAME).get()->uiMovie;

			// Player ActorValues numeric array
			RE::GFxValue playerAVs;
			// Calculated maximum skill limits array for current level
			RE::GFxValue skillCaps;
			// ActionScript object
			RE::GFxValue levelingArgs;
			// Specialization leveling object
			RE::GFxValue spec;
			// Perk only values
			RE::GFxValue perks;

			uiMovie->CreateObject(&levelingArgs);

			levelingArgs.SetMember("skillLevelupsPerLevel", RE::GFxValue(Settings::iMaxLevelsPerSkillPerPlayerLevel));

			int skillPoints = static_cast<int>(Settings::iSkillPointsPerLevel + playerHandle.get()->GetLevel() * Settings::fSkillPointsLevelMultiplier);
			skillPoints += Settings::Saved->RemainingSkillPoints;

			if (skillPoints > Settings::iSkillPointsCap && Settings::iSkillPointsCap > 0)
				skillPoints = Settings::iSkillPointsCap;

			levelingArgs.SetMember("skillPoints", RE::GFxValue(skillPoints));
			levelingArgs.SetMember("cost0", RE::GFxValue(Settings::iSkillPointCost0));
			levelingArgs.SetMember("cost25", RE::GFxValue(Settings::iSkillPointCost25));
			levelingArgs.SetMember("cost50", RE::GFxValue(Settings::iSkillPointCost50));
			levelingArgs.SetMember("cost75", RE::GFxValue(Settings::iSkillPointCost75));

			uiMovie->CreateArray(&playerAVs);
			ScaleformHelper::GetActorBaseAVs(&playerAVs);
			levelingArgs.SetMember("actorValues", playerAVs);

			uiMovie->CreateArray(&skillCaps);
			ScaleformHelper::GetPlayerSkillCaps(&skillCaps);
			levelingArgs.SetMember("skillCaps", skillCaps);

			uiMovie->CreateObject(&perks);
			ScaleformHelper::GetPlayerPerkValues(&perks);
			levelingArgs.SetMember("perkValues", perks);

			uiMovie->CreateObject(&spec);
			if (Settings::selectedSpecIndex != -1)
				Specialization::Specializations[Settings::selectedSpecIndex]->ToGfxValue(&spec, uiMovie);
			levelingArgs.SetMember("specialization", spec);

			uiMovie->Invoke("_root.LevelUpMenu_mc.SetLevelingSettings", nullptr, &levelingArgs, 1);
		} else {
		}
	}

	void RaceMenuHandler::OnMenuChange(bool isOpening)
	{
		if (isOpening)
		{
			if (Settings::Saved->SelectedSpecializationID.length() > 0)
				return;

			RE::GPtr<RE::GFxMovieView> raceMenuMovie = RE::UI::GetSingleton()->GetMenu(RE::RaceSexMenu::MENU_NAME).get()->uiMovie;

			RE::FxResponseArgs<1> classData;
			RE::GFxValue classArray;
			raceMenuMovie->CreateArray(&classArray);

			for (auto it = Specialization::Specializations.begin(); it != Specialization::Specializations.end(); it++) {
				RE::GFxValue data;
				raceMenuMovie->CreateObject(&data);

				it->get()->ToGfxValue(&data, raceMenuMovie);
				classArray.PushBack(data);
			}
			classData.Add(classArray);

			CoreHooks::GfxMovieView_InvokeCallback(raceMenuMovie.get(), "SetClassList", &classData);

			if (Settings::bSSLDebugEnabled)
				SKSE::log::info("Menu {} loaded, {} classes invoked"sv, RE::RaceSexMenu::MENU_NAME, classArray.GetArraySize());
		}
	}

	void SDSScaleform::Register()
	{
		auto ui = RE::UI::GetSingleton();
		ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(SDSScaleform::GetSingleton());
		SKSE::log::info("Registered {} event"sv, typeid(RE::MenuOpenCloseEvent).name());
	}

	SDSScaleform::EventResult SDSScaleform::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		//if (Settings::bSSLDebugEnabled && a_event && a_event->opening)
			// SKSE::log::info("{} is opening", a_event->menuName);

		if (a_event && a_event->menuName == RE::RaceSexMenu::MENU_NAME)
		{
			RaceMenuHandler::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::LevelUpMenu::MENU_NAME)
		{
			LevelUpMenuHandler::OnMenuChange(a_event->opening);
		}
		else if (a_event && a_event->menuName == RE::DialogueMenu::MENU_NAME)
		{
		}
		else if (a_event && a_event->menuName == RE::TweenMenu::MENU_NAME)
		{
		}
		return EventResult::kContinue;
	}

	SDSScaleform::SDSScaleform() :
		_lock()
	{}

}
