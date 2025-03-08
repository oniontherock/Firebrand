#ifndef __OBJECT_ABSTRACTOR_H__
#define __OBJECT_ABSTRACTOR_H__

#include "../include/Common/DataCache.hpp"
#include "../include/Game/World/Objects/ObjectRegistry.hpp"
#include "../include/Game/World/Objects/ObjectTypes.hpp"
#include "../Teams/TeamRelationHolder.hpp"

namespace Goap {
	// forward declare actor
	struct Blackboard;
}

namespace ObjectAbstractor {

	typedef DataCache ObjectData;
	typedef std::vector<ObjectData> ObjectDataVector;
	typedef uint32_t ObjectDataIndex;
	typedef std::set<ObjectDataIndex> ObjectDataIndexVector;

	// fills the given blackboard with the data about the object
	// @param blackboard: the blackboard being filled with the object's data
	// @param entity: a reference to the entity who holds the given blackboard
	// @param object: a reference to the entity/object that we're filling the given blackboard with
	// @param objectType: the object type of the object we're filling the blackboard with
	// @param certainty: how certain we are about this object, I.E. how certain we are about were it is, what it is, etc.
	void blackboardAddObjectData(Goap::Blackboard& blackboard, Entity& entity, Entity& object, ObjectType objectType);

	ObjectData objectBasicDataAbstract(Goap::Blackboard& blackboard, Entity& entity, Entity& object, ObjectType objectType);

	void objectPositionDataAbstract(ObjectData& objectData, Entity& object);
	void animateObjectDataAbstract(Entity& object, ObjectData& objectData);
	void creatureAffiliationAbstract(Goap::Blackboard& blackboard, Entity& entity, Teams::ThreatLevel threatLevel, ObjectData& objectData, ObjectDataIndex objectInd);

	Teams::ThreatLevel objectThreatLevelAssess(EntityId entityId, EntityId objectId);
}

#endif