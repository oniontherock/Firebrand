#include "ObjectRegistry.hpp"


std::vector<ObjectType> ObjectRegistry::objectsVector = std::vector<ObjectType>(MAX_ENTITIES);

void ObjectRegistry::entityObjectTypeAssign(EntityId id, ObjectType objectType) {
	objectsVector[id] = objectType;
}
void ObjectRegistry::entityObjectTypeRemove(EntityId id) {
	entityObjectTypeAssign(id, ObjectType::Null);
}
const ObjectType ObjectRegistry::entityObjectTypeGet(EntityId id) {
	return objectsVector[id];
}
