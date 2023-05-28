#include "Common.h"
#include "Settings.h"

#include <toml.hpp>

namespace SDS
{

	void ReadTomlConfig(std::string path)
	{
		SKSE::log::info("Reading {}", path);

		try {
			const toml::value data = toml::parse(path);
			const std::vector<toml::value> classes = toml::find<std::vector<toml::value>>(data, "class");

			for (auto& c : classes) {
				int i = 0;
				std::string ID = toml::find<toml::string>(c, "id");
				std::string description = toml::find<toml::string>(c, "description");
				SDSFocus focus = static_cast<SDSFocus>(toml::find<toml::integer>(c, "focus"));
				SDSAttribute a1 = static_cast<SDSAttribute>(toml::find<toml::integer>(c, "attr1"));
				SDSAttribute a2 = static_cast<SDSAttribute>(toml::find<toml::integer>(c, "attr2"));

				Specialization::Specializations.push_back(std::make_unique<Specialization>(ID, fmt::format("${}", ID), description, focus, a1, a2));
				Specialization* spec = Specialization::Specializations.back().get();

				std::vector<int> skills = toml::find<std::vector<int>>(c, "skills");
				for (i = 0; i < skills.size(); i++) {
					spec->SetSkillFocus(skills[i], i);
				}
			}
		} catch (const std::exception& e) {
			util::report_and_fail(e.what());
		} catch (...) {
			util::report_and_fail("Configuration file incomplete"sv);
		}

		SKSE::log::info("Reading {} completed, loaded {} classes", path, Specialization::Specializations.size());
	}

	void Settings::Initialize()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			// esp variables lookup
		}
	}

	void Settings::ReadSettings()
	{
		const std::string dataPath("Data/SKSE/Plugins/SDS.toml");
		// Load class data and mod settings from toml file
		ReadTomlConfig(dataPath);
		/*
			TODO, load mod settings
		*/
	}

	void Settings::OnPostLoadGame()
	{

	}

	/* TODO, mcm
	void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
	{
		(void)a_ini;
		(void)a_sectionName;
		(void)a_settingName;
		(void)a_setting;
	}

	void Settings::ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
	{
		(void)a_ini;
		(void)a_sectionName;
		(void)a_settingName;
		(void)a_setting;
	}

	void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
	{
		(void)a_ini;
		(void)a_sectionName;
		(void)a_settingName;
		(void)a_setting;
	}
	*/
}
