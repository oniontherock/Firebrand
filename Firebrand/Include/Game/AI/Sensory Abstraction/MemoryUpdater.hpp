#ifndef __MEMORY_UPDATER_H__
#define __MEMORY_UPDATER_H__

#include "ObjectAbstractor.hpp"

namespace Goap {
	// forward declare Blackboard
	struct Blackboard;
}

namespace MemoryUpdater {
	void memoryUpdate(Goap::Blackboard& memory, ObjectAbstractor::ObjectDataVector memoryDataNew);

	// updates the various sets in the memory, like the "threats" set or "creatures" set,
	// note that the "objects" variable is NOT modified here, that is done in memoryUpdate
	void objectDesignationsUpdate(Goap::Blackboard& memory);

	void objectAnimateDesignate(Goap::Blackboard& memory, const ObjectData& objectData, ObjectAbstractor::ObjectDataIndex objectInd);

	float objectGetCertainty(ObjectData& object);
}



#endif