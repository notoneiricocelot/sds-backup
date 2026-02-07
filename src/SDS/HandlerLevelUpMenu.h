#pragma once

namespace SDS
{
	class HandlerLevelUpMenu
	{
		public:
			static void OnMenuChange(bool isOpening);

			static void CompleteLevelUp(const RE::FxDelegateArgs& a_params);
			static void CalculatePlayerStatsForAttribute(const RE::FxDelegateArgs& a_params);
			static void HighlightPlayerCard(const RE::FxDelegateArgs& a_params);

			static void AddStats();
	};

}
