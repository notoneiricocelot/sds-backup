#pragma once

#include <RE/Skyrim.h>

namespace SDS {
    bool RegisterSDSInterface(RE::BSScript::IVirtualMachine* vm);

    void InitializeHook(SKSE::Trampoline& trampoline);
}
