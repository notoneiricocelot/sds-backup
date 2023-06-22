#pragma once

namespace SDS
{

	class LevelUpMenuHandler
	{
		public:
			static void OnMenuChange(bool isOpening);

			static void AddStats();

			static constexpr char CompleteLevelUp_Name[] = "completeLevelUp";
			static void CompleteLevelUp(const RE::FxDelegateArgs& a_params);
	};
}
