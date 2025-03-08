#ifndef __GOAP_BLACKBOARD_H__
#define __GOAP_BLACKBOARD_H__

#include "../../Sensory Abstraction/ObjectAbstractor.hpp"
#include "../include/Common/DataCache.hpp"
#include <vector>

namespace Goap {
	struct Blackboard : DataCache {

		Blackboard();

		ObjectAbstractor::ObjectDataVector objects;
		std::unordered_map<EntityId, ObjectAbstractor::ObjectDataIndex> objectIds;

		ObjectAbstractor::ObjectDataIndex objectAdd(ObjectAbstractor::ObjectData& objectData);
		void objectRemove(ObjectAbstractor::ObjectDataIndex ind);
		bool objectHas(EntityId objectId) const;
		void objectUpdate(ObjectAbstractor::ObjectData& objectData);

	protected:
		std::set<ObjectAbstractor::ObjectDataIndex> availableObjectIds;
	};
}

#endif