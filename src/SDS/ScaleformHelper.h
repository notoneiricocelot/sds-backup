#pragma once

namespace SDS
{
	struct ScaleformHelper
	{
	public:
		static void GetPlayerObject(RE::GFxValue* playerObject);
		static void GetBottomBarObjectDefault(RE::GFxValue* bottombarObject);
		static void GetActorBaseAVs(RE::GFxValue* av_list);
		static void GetPlayerSkillCaps(RE::GFxValue* av_skills);

	private:
		using AV = RE::ActorValue;
	};

}
