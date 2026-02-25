#pragma once

namespace SDS
{
	class BonfireAnimationGraphManager : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
	{
		public:
			static BonfireAnimationGraphManager* GetSingleton();

			RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent*, RE::BSTEventSource<RE::BSAnimationGraphEvent>*) override;
	};

}
