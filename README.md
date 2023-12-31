## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [The Elder Scrolls V: Skyrim Special Edition](https://store.steampowered.com/app/489830)
	* Add the environment variable `CompiledPluginsPath` to point to the folder where you want the .dll to be copied after building
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG/tree/v3.4.0)
	* Add the environment variable `CommonLibSSEPath_NG` with the value as the path to the folder containing CommonLibSSE-NG

## User Requirements
* [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Building
```
git clone
cd SDS
git submodule init
git submodule update

cmake --preset vs2022-windows
cmake --build build --config Release
```

## Credits

https://github.com/ianpatt/skse64/tree/master
https://github.com/Ryan-rsm-McKenzie/CommonLibSSE
https://github.com/Mardoxx/skyrimui
https://github.com/ersh1/TrueDirectionalMovement/blob
https://gitlab.com/colorglass/commonlibsse-sample-plugin
