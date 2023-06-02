#pragma once

#include <RE/Skyrim.h>

#include "SDSClassMenu.h"

namespace SDS
{
	/**
	 *
	 * <p>
	 * All aspects of this are done in a singleton since SKSE cannot create instances of new Papyrus types such as a
	 * hit counter.
	 * </p>
	 */
	class SDSScaleform : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{

		private:
			using EventResult = RE::BSEventNotifyControl;
			using Lock = std::recursive_mutex;
			using Locker = std::lock_guard<Lock>;

		public:
			/**
			 * Get the singleton instance of the <code>SDSInterface</code>.
			 */
			static SDSScaleform* GetSingleton()
			{
				static SDSScaleform singleton;
				return std::addressof(singleton);
			}

			static void Register();

			virtual EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

		private:
			mutable Lock _lock;

			SDSScaleform();
			SDSScaleform(const SDSScaleform&) = delete;
			SDSScaleform(SDSScaleform&&) = delete;

			~SDSScaleform() = default;

			SDSScaleform& operator=(const SDSScaleform&) = delete;
			SDSScaleform& operator=(SDSScaleform&&) = delete;
	};
}  // namespace SDS
