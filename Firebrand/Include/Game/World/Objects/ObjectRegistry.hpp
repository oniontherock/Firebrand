#ifndef __OBJECT_REGISTRY_H__
#define __OBJECT_REGISTRY_H__

#include "ObjectTypes.hpp"
#include <ECS/TypeDefinitions.hpp>
#include <vector>

struct SaveDirector;

// holds a vector of every entity's ObjectType,
// if an entity doesn't have a type, it is marked as ObjectTypes::Null.
struct ObjectRegistry {
	// assigns an ObjectType to an EntityId
	static void entityObjectTypeAssign(EntityId id, ObjectType objectType);
	// sets an entity's ObjectType to ObjectTypes::Null
	static void entityObjectTypeRemove(EntityId id);
	// assigns an ObjectType to an EntityId
	static const ObjectType entityObjectTypeGet(EntityId id);

	friend struct SaveDirector;

private:
	// list of ObjectTypes accessed by EntityIds.
	// each entity can have only one ObjectType, not every entity must have an ObjectType.
	// objectsVector is of size MAX_ENTITIES
	static std::vector<ObjectType> objectsVector;
};


#endif