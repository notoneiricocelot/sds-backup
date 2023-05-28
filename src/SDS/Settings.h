#pragma once

#include "Common.h"

namespace SDS
{

	struct Settings
	{
		static void Initialize();
		static void ReadSettings();
		static void OnPostLoadGame();

		static int LogEnabled;
		/* TODO MCM
		static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
		static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
		static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
		*/
	};
}
