#pragma once

namespace SDS
{
	RE::MessageBoxData* NewMessageBox(const std::string& a_message);

	int GetSkillLevelCap(RE::PlayerCharacter*);
	int GetSkillLevelCap(RE::PlayerCharacter*, RE::ActorValue);

	int GetSkillpointCost(int skillLevel);

	struct ScaleformHelper
	{
	private:
		using AV = RE::ActorValue;

	public:
		static void GetPlayerObject(RE::GFxValue* playerObject);
		static void GetActorBaseAVs(RE::GFxValue* av_list);
		static void GetPlayerSkillCaps(RE::GFxValue* av_skills);
		static void GetPlayerPerkValues(RE::GFxValue* perkValues);
		static void GetAttributes(RE::GFxValue* attributes);
	};

}
