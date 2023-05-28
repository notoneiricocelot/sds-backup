#include "Papyrus.h"
#include "SDSScaleform.h"
#include "Settings.h"

#include <stddef.h>

namespace
{
	/**
	 * Setup logging.
	 *
	 * <p>
	 * Logging is important to track issues. CommonLibSSE bundles functionality for spdlog, a common C++ logging
	 * framework. Here we initialize it, using values from the configuration file. This includes support for a debug
	 * logger that shows output in your IDE when it has a debugger attached to Skyrim, as well as a file logger which
	 * writes data to the standard SKSE logging directory at <code>Documents/My Games/Skyrim Special Edition/SKSE</code>
	 * (or <code>Skyrim VR</code> if you are using VR).
	 * </p>
	 */
	void InitializeLogging()
	{
		auto path = SKSE::log::log_directory();
		if (!path) {
			SKSE::stl::report_and_fail("Unable to lookup SKSE logs directory.");
		}
		*path /= SKSE::PluginDeclaration::GetSingleton()->GetName();
		*path += L".log";

		std::shared_ptr<spdlog::logger> log;
		if (IsDebuggerPresent()) {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
		} else {
			log = std::make_shared<spdlog::logger>(
				"Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
		}
		log->set_level(spdlog::level::level_enum::info);
		log->flush_on(spdlog::level::level_enum::trace);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
	}

	/**
	 * Initialize the SKSE cosave system for our plugin.
	 *
	 * <p>
	 * SKSE comes with a feature called a <em>cosave</em>, an additional save file kept alongside the original Skyrim
	 * save file. SKSE plugins can write their own data to this file, and load it again when the save game is loaded,
	 * allowing them to keep custom data along with a player's save. Each plugin must have a unique ID, which is four
	 * characters long (similar to the record names used by forms in ESP files). Note however this is little-endian, so
	 * technically the 'SMPL' here ends up as 'LPMS' in the save file, unless we use a byte order swap.
	 * </p>
	 *
	 * <p>
	 * There can only be one serialization callback for save, revert (called on new game and before a load), and load
	 * for the entire plugin.
	 * </p>
	 */
	void InitializeSerialization()
	{
		SKSE::log::trace("Initializing cosave serialization...");
		auto* serde = SKSE::GetSerializationInterface();
		serde->SetUniqueID(_byteswap_ulong('SDSI'));
		serde->SetSaveCallback(SDS::SDSScaleform::OnGameSaved);
		serde->SetRevertCallback(SDS::SDSScaleform::OnRevert);
		serde->SetLoadCallback(SDS::SDSScaleform::OnGameLoaded);
		SKSE::log::trace("Cosave serialization initialized.");
	}

	/**
	 * Initialize our Papyrus extensions.
	 *
	 * <p>
	 * A common use of SKSE is to add new Papyrus functions. You can call a registration callback to do this. This
	 * callback will not necessarily be called immediately, if the Papyrus VM has not been initialized yet (in that case
	 * it's execution is delayed until the VM is available).
	 * </p>
	 *
	 * <p>
	 * You can call the <code>Register</code> function as many times as you want and at any time you want to register
	 * additional functions.
	 * </p>
	 */
	void InitializePapyrus()
	{
		// RE::BSScript
		SKSE::log::trace("Initializing Papyrus binding...");
		if (SKSE::GetPapyrusInterface()->Register(SDS::RegisterSDSInterface)) {
			SKSE::log::debug("Papyrus functions bound.");
		} else {
			SKSE::stl::report_and_fail("Failure to register Papyrus bindings.");
		}
	}

	/**
	 * Initialize the trampoline space for function hooks.
	 *
	 * <p>
	 * Function hooks are one of the most powerful features available to SKSE developers, allowing you to replace
	 * functions with your own, or replace a function call with a call to another function. However, to do this, we
	 * need a code snippet that replicates the first instructions of the original code we overwrote, in order to be
	 * able to call back to the original control flow with all the same functionality.
	 * </p>
	 *
	 * <p>
	 * CommonLibSSE comes with functionality to allocate trampoline space, including a common singleton space we can
	 * access from anywhere. While this is not necessarily the most advanced use of trampolines and hooks, this will
	 * suffice for our demo project.
	 * </p>
	 */
	void InitializeHooking()
	{
		SDS::InitializeHook();
	}

	void InitializeScaleform()
	{
		SDS::SDSScaleform::Register();
	}

	/**
	 * Register to listen for messages.
	 *
	 * <p>
	 * SKSE has a messaging system to allow for loosely coupled messaging. This means you don't need to know about or
	 * link with a message sender to receive their messages. SKSE itself will send messages for common Skyrim lifecycle
	 * events, such as when SKSE plugins are done loading, or when all ESP plugins are loaded.
	 * </p>
	 *
	 * <p>
	 * Here we register a listener for SKSE itself (because we have not specified a message source). Plugins can send
	 * their own messages that other plugins can listen for as well, although that is not demonstrated in this example
	 * and is not common.
	 * </p>
	 *
	 * <p>
	 * The data included in the message is provided as only a void pointer. It's type depends entirely on the type of
	 * message, and some messages have no data (<code>dataLen</code> will be zero).
	 * </p>
	 */
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		// Skyrim lifecycle events.
		// Called after all plugins have finished running SKSEPlugin_Load.
		// It is now safe to do multithreaded operations, or operations against other plugins.
		case SKSE::MessagingInterface::kPostLoad:
		// Called after all kPostLoad message handlers have run.
		case SKSE::MessagingInterface::kPostPostLoad:
		// Called when all game data has been found.
		case SKSE::MessagingInterface::kInputLoaded:
			break;
		// All ESM/ESL/ESP plugins have loaded, main menu is now active.
		case SKSE::MessagingInterface::kDataLoaded:
			// It is now safe to access form data.
			// Read our settings file and prepare data
			SDS::Settings::Initialize();
			SDS::Settings::ReadSettings();
			// Init hooks (nothing here at least now, just for future upadtes)
			InitializeHooking();
			// Init user interface callbacks
			InitializeScaleform();
			break;
		// Skyrim game events.
		// Player starts a new game from main menu.
		case SKSE::MessagingInterface::kNewGame:
		// Player selected a game to load, but it hasn't loaded yet.
		case SKSE::MessagingInterface::kPreLoadGame:
		// Data will be the name of the loaded save.
		// Player's selected save game has finished loading.
		case SKSE::MessagingInterface::kPostLoadGame:
		// Data will be a boolean indicating whether the load was successful.
		// The player has saved a game.
		case SKSE::MessagingInterface::kSaveGame:
		// Data will be the save name.
		// The player deleted a saved game from within the load menu.
		case SKSE::MessagingInterface::kDeleteGame:
			break;
		}
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION[0];

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_SSE_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;

	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);
	v.AuthorName("ThanksYouErsh");
	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
	v.HasNoStructUse(true);

	return v;
}();


/**
 * This if the main callback for initializing your SKSE plugin, called just before Skyrim runs its main function.
 *
 * <p>
 * This is your main entry point to your plugin, where you should initialize everything you need. Many things can't be
 * done yet here, since Skyrim has not initialized and the Windows loader lock is not released (so don't do any
 * multithreading). But you can register to listen for messages for later stages of Skyrim startup to perform such
 * tasks.
 * </p>
 */
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLogging();
	auto* plugin = SKSE::PluginDeclaration::GetSingleton();
	auto version = plugin->GetVersion();
	SKSE::log::info("{} {} is loading...", plugin->GetName(), version);

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(static_cast<int64_t>(1 << 8));

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	InitializeSerialization();
	InitializePapyrus();

	SKSE::log::info("{} has finished loading.", plugin->GetName());
	return true;
}
