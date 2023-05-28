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

			static void Register();
			static bool RegisterScaleform(RE::GFxMovieView* a_view, RE::GFxValue* a_root);

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
