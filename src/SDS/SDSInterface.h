#pragma once

#include <RE/Skyrim.h>

namespace SDS {
#pragma warning(push)
#pragma warning(disable : 4251)
    /**
     *
     * <p>
     * All aspects of this are done in a singleton since SKSE cannot create instances of new Papyrus types such as a
     * hit counter.
     * </p>
     */
    class __declspec(dllexport) SDSInterface {
    public:
        /**
         * Get the singleton instance of the <code>SDSInterface</code>.
         */
        [[nodiscard]] static SDSInterface& GetSingleton() noexcept;

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

    private:
        SDSInterface() = default;

        mutable std::mutex _lock;
    };
#pragma warning(pop)
}  // namespace SDS
