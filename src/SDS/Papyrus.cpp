#include "Papyrus.h"

namespace SDS
{
	/**
	 * This is the function that acts as a registration callback for Papyrus functions. Within you can register functions
	 * to bind to native code. The first argument of such bindings is the function name, the second is the class name, and
	 * third is the function that will be invoked in C++ to handle it. The callback should return <code>true</code> on
	 * success.
	 */
	bool SDS::RegisterSDSInterface(RE::BSScript::IVirtualMachine* vm)
	{
		// vm->RegisterFunction("GetTotalHitCounters", PapyrusClass, SetClass);
		(void)vm;
		return true;
	}

	void SDS::InitializeHook()
	{
		// The trampoline can be used to write a new call instruction at a given address (here the start of the function for
		// HitData::Populate). We use write_code<5> to indicate this is a 5-byte call instruction (rather than the much
		// rarer 6-byte call). We pass in the address of our function that will be called, and a pointer to the trampoline
		// function is returned.
		//
		// The trampoline pointed to contains any instructions from the original function we overwrote and a call to the
		// instruction that comes after, so that if we call that address as a function, we are in effect calling the
		// original code.
		// OriginalPopulateHitData = trampoline.write_call<5>(GetHookedFunction().address(),
			//												reinterpret_cast<uintptr_t>(PopulateHitData));
		SKSE::log::debug("Papyrus hook called.");
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
