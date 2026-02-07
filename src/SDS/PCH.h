#pragma once

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>
#include "SKSE/Logger.h"

#include <Windows.h>

#undef cdecl // Workaround for Clang 14 CMake configure error.

#include <SimpleIni.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

// Compatible declarations with other sample projects.
#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;
using namespace REL::literals;

namespace logger = SKSE::log;

namespace util {
	using SKSE::stl::report_and_fail;
}

#ifndef  RELOCATION_OFFSET
#define RELOCATION_OFFSET(SE, AE) REL::VariantOffset(SE, AE, 0).offset()
#endif  // ! 


#include "Plugin.h"
