#pragma once

namespace SDS
{

	constexpr std::string_view PapyrusClass = "SDSInterface";

	class OnClassAcceptedHandler : public SKSE::RegistrationSet<INT32>
	{
		public:
			static OnClassAcceptedHandler* GetSingleton();

			static constexpr char EVN_NAME[] = "SDSClassSelected";
		private:
			using Base = SKSE::RegistrationSet<INT32>;

			OnClassAcceptedHandler();
			OnClassAcceptedHandler(const OnClassAcceptedHandler&) = delete;
			OnClassAcceptedHandler(OnClassAcceptedHandler&&) = delete;
			~OnClassAcceptedHandler() = default;

			OnClassAcceptedHandler& operator=(const OnClassAcceptedHandler&) = delete;
			OnClassAcceptedHandler& operator=(OnClassAcceptedHandler&&) = delete;

	};

	class OpenLevelUpMenuHandler : public SKSE::RegistrationSet<INT32>
	{
		public:
		static OpenLevelUpMenuHandler* GetSingleton();

			static constexpr char EVN_NAME[] = "ShowLevelUpMenu";
		private:
			using Base = SKSE::RegistrationSet<INT32>;

			OpenLevelUpMenuHandler();
			OpenLevelUpMenuHandler(const OpenLevelUpMenuHandler&) = delete;
			OpenLevelUpMenuHandler(OpenLevelUpMenuHandler&&) = delete;
			~OpenLevelUpMenuHandler() = default;

			OpenLevelUpMenuHandler& operator=(const OpenLevelUpMenuHandler&) = delete;
			OpenLevelUpMenuHandler& operator=(OpenLevelUpMenuHandler&&) = delete;

	};

	class SDSInterface
	{
		public:
			static void SDSClassSelected(RE::StaticFunctionTag*, std::string name);
			static void ShowLevelUpMenu(RE::StaticFunctionTag*);

			static bool Register(RE::BSScript::IVirtualMachine* vm);

			static constexpr char PapyrusClass[] = "SDSInterface";
	};
}
