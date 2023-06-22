#include "Papyrus.h"

namespace SDS
{

	/**
	 * This is the function that acts as a registration callback for Papyrus functions. Within you can register functions
	 * to bind to native code. The first argument of such bindings is the function name, the second is the class name, and
	 * third is the function that will be invoked in C++ to handle it. The callback should return <code>true</code> on
	 * success.
	 */
	bool SDSInterface::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction(OnClassAcceptedHandler::EVN_NAME, PapyrusClass, SDSClassSelected);
		return true;
	}

	void SDSInterface::SDSClassSelected(RE::StaticFunctionTag*, [[maybe_unused]] std::string name)
	{

	}

	OnClassAcceptedHandler* OnClassAcceptedHandler::GetSingleton()
	{
		static OnClassAcceptedHandler singleton;
		return &singleton;
	}

	OnClassAcceptedHandler::OnClassAcceptedHandler() :
		Base(EVN_NAME)
	{}

}
