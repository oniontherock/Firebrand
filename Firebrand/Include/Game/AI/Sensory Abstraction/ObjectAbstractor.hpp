#ifndef __OBJECT_ABSTRACTOR_H__
#define __OBJECT_ABSTRACTOR_H__

#include "../include/Common/DataCache.hpp"
#include "../include/Game/World/Objects/ObjectRegistry.hpp"
#include "../include/Game/World/Objects/ObjectTypes.hpp"
#include "../Teams/TeamRelationHolder.hpp"

typedef DataCache ObjectData;
typedef std::vector<ObjectData> ObjectDataVector;
typedef uint32_t ObjectDataIndex;

namespace ObjectAbstractor {
	ObjectData objectDataAbstract(EntityId objectId, ObjectType objectType);

	void animateObjectDataAbstract(Entity& object, ObjectData& objectData);

	Teams::ThreatLevel objectThreatLevelAssess(EntityId entityId, EntityId objectId);
}

#endif