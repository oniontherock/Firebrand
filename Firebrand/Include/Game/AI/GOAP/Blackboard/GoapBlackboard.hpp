#ifndef __GOAP_BLACKBOARD_H__
#define __GOAP_BLACKBOARD_H__

#include "../../Sensory Abstraction/ObjectAbstractor.hpp"
#include "../include/Common/DataCache.hpp"
#include <vector>

namespace Goap {
	struct Blackboard : DataCache {
		ObjectDataVector objects;

		std::set<ObjectDataIndex> creatures;
		std::set<ObjectDataIndex> threats;
		ObjectDataIndex threatClosest;
		std::set<ObjectDataIndex> allies;
		std::set<ObjectDataIndex> items;
		std::set<ObjectDataIndex> obstacles;

		ObjectDataIndex objectAdd(ObjectData& objectData);
	};
}

#endif