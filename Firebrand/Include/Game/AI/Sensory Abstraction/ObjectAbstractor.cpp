#include "ObjectAbstractor.hpp"

using namespace EntityComponents;

ObjectData ObjectAbstractor::objectDataAbstract(EntityId objectId, ObjectType objectType) {

	Entity& object = EntityManager::entityGet(objectId);

	ObjectData objectData;

	objectData.dataSet("ObjectType", objectType);
	if (object.entityComponentHas<ComponentPosition>()) objectData.dataSet("Position", object.entityComponentGet<ComponentPosition>()->position);
	if (object.entityComponentHas<ComponentRotation>()) objectData.dataSet("Rotation", object.entityComponentGet<ComponentRotation>()->rotation);
	if (object.entityComponentHas<ComponentActorStateHolder>()) objectData.dataSet("State", object.entityComponentGet<ComponentActorStateHolder>()->actorStateHolder);
	objectData.dataSet("HasSenseSight", object.entityComponentHas<ComponentObjectVision>());
	objectData.dataSet("IsAnimate", false);

	return objectData;
}

bool ObjectAbstractor::objectThreatLevelAssess(EntityId entityId, EntityId objectId) {
	Entity& entity = EntityManager::entityGet(entityId);
	Entity& object = EntityManager::entityGet(objectId);

	if (!entity.entityComponentHas<ComponentTeam>()) return false;
	if (!object.entityComponentHas<ComponentTeam>()) return false;

	Teams::TeamId entityTeamId = entity.entityComponentGet<ComponentTeam>()->teamId;
	Teams::TeamId objectTeamId = object.entityComponentGet<ComponentTeam>()->teamId;

	if (entityTeamId == objectTeamId) return false;

	Teams::TeamRelationValue teamRelation = Teams::TeamRelationHolder::teamRelationGet(entityTeamId, objectTeamId);

	if (teamRelation < -50.f) return true;

	return false;
}
