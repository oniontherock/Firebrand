#ifndef __GOAP_BLACKBOARD_H__
#define __GOAP_BLACKBOARD_H__

#include "../../Sensory Abstraction/ObjectAbstractor.hpp"
#include "../include/Common/DataCache.hpp"
#include <vector>

namespace Goap {
	struct Blackboard : DataCache {

		Blackboard();

		ObjectAbstractor::ObjectDataVector objects;

		ObjectAbstractor::ObjectDataIndex objectAdd(ObjectAbstractor::ObjectData& objectData);
	};
}

#endif