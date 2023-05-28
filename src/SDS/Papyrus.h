#pragma once

#include "PCH.h"

namespace SDS
{
	bool RegisterSDSInterface(RE::BSScript::IVirtualMachine* vm);

	void InitializeHook();

	constexpr std::string_view PapyrusClass = "SDSInterface";

	class OnClassAcceptedHandler : public SKSE::RegistrationSet<INT32>
	{
		public:
		static OnClassAcceptedHandler* GetSingleton();

		private:
			using Base = SKSE::RegistrationSet<INT32>;

			OnClassAcceptedHandler();
			OnClassAcceptedHandler(const OnClassAcceptedHandler&) = delete;
			OnClassAcceptedHandler(OnClassAcceptedHandler&&) = delete;
			~OnClassAcceptedHandler() = default;

			OnClassAcceptedHandler& operator=(const OnClassAcceptedHandler&) = delete;
			OnClassAcceptedHandler& operator=(OnClassAcceptedHandler&&) = delete;

			static constexpr char EVN_NAME[] = "SDSClassAccepted";
	};
}
