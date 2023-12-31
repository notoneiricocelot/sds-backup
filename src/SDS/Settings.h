#pragma once

#include "Common.h"

namespace SDS
{

	struct Settings
	{
		using Lock = std::recursive_mutex;
		using Locker = std::lock_guard<Lock>;

		static void Initialize();
		static void ReadSettings();
		static void OnPostLoadGame();

		/*
			Game Settings
		*/
		static inline int selectedSpecIndex = -1;

		/*
			Sezialization ID for Save/Load serialization
		*/
		static inline const auto SpecializationRecord = _byteswap_ulong('SPEC');

		/*
			MCM leveling settings
			[General]
		*/
		static inline bool bSSLDebugEnabled = false;

		static inline int iMaxLevelsPerSkillPerPlayerLevel = 5;
		static inline int iSkillPointCost0 = 3;
		static inline int iSkillPointCost25 = 5;
		static inline int iSkillPointCost50 = 7;
		static inline int iSkillPointCost75 = 9;
		static inline int iSkillPointsPerLevel = 15;
		static inline int iSkillPointsCap = 45;
		static inline int iBaseAttributeValue = 20;
		static inline int iClassBaseAttributeBonus = 5;
		static inline int iClassBaseSkillBonus = 5;

		static inline float fSkillPointsLevelMultiplier = 1.5f;

		/*
			Experience settings
			https://www.nexusmods.com/skyrimspecialedition/mods/17751?tab=description
			[General]
		*/
		static inline float fSkillCapBase_Experience = 18.0f;
		static inline float fSkillCapMult_Experience = 2.0f;
		static inline int iMaxPlayerLevel_Experience = 255;
		static inline bool bUseRacialCaps_Experience = true;

		static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
		static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
		static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);

		/**
			* The serialization handler for reverting game state.
			*
			* <p>
			* This is called as the handler for revert. Revert is called by SKSE on a plugin that is registered for
			* serialization handling on a new game or before a save game is loaded. It should be used to revert the state
			* of the plugin back to its default.
			* </p>
			*/
		static void OnRevert(SKSE::SerializationInterface*);

		/**
			 * The serialization handler for saving data to the cosave.
			 *
			 * @param serde The serialization interface used to write data.
			 */
		static void OnGameSaved(SKSE::SerializationInterface* serde);

		/**
			 * The serialization handler for loading data from a cosave.
			 *
			 * @param serde  The serialization interface used to read data.
			 */
		static void OnGameLoaded(SKSE::SerializationInterface* serde);

		static inline std::vector<SDSLeveledAttribute> LevelingSettings;

		static inline std::unique_ptr<PlayerData> Saved = std::make_unique<PlayerData>();

		static inline std::mutex _lock;
	};
}
