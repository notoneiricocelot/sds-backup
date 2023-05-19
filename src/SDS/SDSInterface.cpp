#include <SKSE/SKSE.h>
#include "SDSInterface.h"

using namespace RE;
using namespace SDS;
using namespace SKSE;

namespace {
    // These four-character record types, which store data in the SKSE cosave, are little-endian. That means they are
    // reversed from the character order written here. Using the byteswap functions reverses them back to the order
    // the characters are written in the source code.
}

SDSInterface& SDSInterface::GetSingleton() noexcept {
    static SDSInterface instance;
    return instance;
}


/*

 */
void SDSInterface::OnRevert(SerializationInterface*) {
    std::unique_lock lock(GetSingleton()._lock);
	//
}

void SDSInterface::OnGameLoaded(SerializationInterface* serde) {
    std::uint32_t type;
    std::uint32_t size;
    std::uint32_t version;
    
	(void)type;
	(void)size;
	(void)version;
	(void)serde;
	// To load records from a cosave, use <code>GetNextRecordInfo</code> to iterate from one record to the next.
    // You will be given records in the order they were written, but otherwise you do not look up a record by its name.
    // Instead check the result of each iteration to find out which record it is and handle it appropriately.
    //
    // If you make breaking changes to your data format, you can increase the version number used when writing the data
    // out and check that number here to handle previous versions.
    // while (serde->GetNextRecordInfo(type, version, size)) {
    // }
}

void SDSInterface::OnGameSaved(SerializationInterface* serde) {
    std::unique_lock lock(GetSingleton()._lock);
	(void)serde;
    // To write data open a record with a given name. The name must be unique within your mod, and has a version number
    // assigned (in this case 0). You can increase the version number if making breaking format change from a previous
    // version of your mod, so that the load handler can know which version of the format is being used.
}
