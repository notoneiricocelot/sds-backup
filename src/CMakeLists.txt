set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(SOURCE_DIR "${ROOT_DIR}/src")
set(SOURCE_FILES
	"${SOURCE_DIR}/SDS/Main.cpp"
	"${SOURCE_DIR}/SDS/Common.cpp"
	"${SOURCE_DIR}/SDS/Common.h"
	"${SOURCE_DIR}/SDS/Specialization.cpp"
	"${SOURCE_DIR}/SDS/Specialization.h"
	"${SOURCE_DIR}/SDS/Papyrus.cpp"
	"${SOURCE_DIR}/SDS/Papyrus.h"
	"${SOURCE_DIR}/SDS/HandlerAnimGraphs.cpp"
	"${SOURCE_DIR}/SDS/HandlerAnimGraphs.h"
	"${SOURCE_DIR}/SDS/HandlerLevelUpMenu.cpp"
	"${SOURCE_DIR}/SDS/HandlerLevelUpMenu.h"
	"${SOURCE_DIR}/SDS/HandlerRaceSexMenu.cpp"
	"${SOURCE_DIR}/SDS/HandlerRaceSexMenu.h"
	"${SOURCE_DIR}/SDS/HandlerTweenMenu.cpp"
	"${SOURCE_DIR}/SDS/HandlerTweenMenu.h"
	"${SOURCE_DIR}/SDS/BonfireMenu.cpp"
	"${SOURCE_DIR}/SDS/BonfireMenu.h"
	"${SOURCE_DIR}/SDS/EmoteMenu.cpp"
	"${SOURCE_DIR}/SDS/EmoteMenu.h"
	"${SOURCE_DIR}/SDS/Scaleform.cpp"
	"${SOURCE_DIR}/SDS/Scaleform.h"
	"${SOURCE_DIR}/SDS/ScaleformHelper.cpp"
	"${SOURCE_DIR}/SDS/ScaleformHelper.h"
	"${SOURCE_DIR}/SDS/HooksCore.cpp"
	"${SOURCE_DIR}/SDS/HooksCore.h"
	"${SOURCE_DIR}/SDS/HooksUI.cpp"
	"${SOURCE_DIR}/SDS/HooksUI.h"
	"${SOURCE_DIR}/SDS/Settings.cpp"
	"${SOURCE_DIR}/SDS/Settings.h"
	"${SOURCE_DIR}/SDS/PCH.h"
)

source_group(TREE "${ROOT_DIR}" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
	"${ROOT_DIR}/cmake/Plugin.h.in"
	"${VERSION_HEADER}"
	@ONLY
)

source_group("src" FILES "${VERSION_HEADER}")

configure_file(
	"${ROOT_DIR}/cmake/version.rc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	@ONLY
)

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
	"${VERSION_HEADER}"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	"${ROOT_DIR}/.clang-format"
	"${ROOT_DIR}/.editorconfig"
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_20
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			# "/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4200" # nonstandard extension used : zero-sized array in struct/union
			"/DUNICODE"
			"/D_UNICODE"

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob2>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:NONE>"
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	SYSTEM PRIVATE
		"${CMAKE_SOURCE_DIR}/include"
)

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/src"
		"${SOURCE_DIR}"
)

set(SKSE_SUPPORT_XBYAK ON)
add_subdirectory("$ENV{CommonLibSSEPath_NG}" CommonLibSSE SYSTEM EXCLUDE_FROM_ALL)

find_package(xbyak REQUIRED CONFIG)
find_package(toml11 REQUIRED CONFIG)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		CommonLibSSE::CommonLibSSE
		toml11::toml11
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"${SOURCE_DIR}/SDS/PCH.h"
)

install(
	FILES
		"$<TARGET_FILE:${PROJECT_NAME}>"
	DESTINATION "SKSE/Plugins"
	COMPONENT "main"
)

if("${COPY_OUTPUT}")
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${CompiledPluginsPath}/SKSE/Plugins/"
		VERBATIM
	)
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${ROOT_DIR}/cmake/sds.toml" "${CompiledPluginsPath}/SKSE/Plugins/"
		VERBATIM
	)
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${ROOT_DIR}/translation/SDSInterface_english.txt" "${CompiledPluginsPath}/Interface/Translations/"
		VERBATIM
	)
	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${ROOT_DIR}/translation/SDSInterface_russian.txt" "${CompiledPluginsPath}/Interface/Translations/"
		VERBATIM
	)
endif()
