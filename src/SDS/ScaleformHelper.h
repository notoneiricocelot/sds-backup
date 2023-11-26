#pragma once

namespace SDS
{
	struct ScaleformHelper
	{
	private:
		using AV = RE::ActorValue;

	public:
		static void GetPlayerObject(RE::GFxValue* playerObject);
		static void GetBottomBarObjectDefault(RE::GFxValue* bottombarObject);
		static void GetActorBaseAVs(RE::GFxValue* av_list);
		static void GetPlayerSkillCaps(RE::GFxValue* av_skills);
		static void GetPlayerPerkValues(RE::GFxValue* perkValues);
		static void GetAttributes(RE::GFxValue* attributes);
	};

}
