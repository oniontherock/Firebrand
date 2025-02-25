#include "ObjectAbstractor.hpp"
#include "../ACECS/ECSRegistry.hpp"

using namespace EntityComponents;

ObjectData ObjectAbstractor::objectDataAbstract(EntityId objectId, ObjectType objectType) {

	Entity& object = EntityManager::entityGet(objectId);

	ObjectData objectData;

	objectData.dataSet("ObjectType", objectType);
	if (object.entityComponentHas<ComponentPosition>()) objectData.dataSet("Position", object.entityComponentGet<ComponentPosition>()->position);
	if (object.entityComponentHas<ComponentRotation>()) objectData.dataSet("Rotation", object.entityComponentGet<ComponentRotation>()->rotation);

	bool objectIsAnimate = object.entityComponentGet<ComponentIsAnimate>();
	objectData.dataSet("IsAnimate", objectIsAnimate);

	if (objectIsAnimate) {
		animateObjectDataAbstract(object, objectData);
	}

	return objectData;
}
void ObjectAbstractor::animateObjectDataAbstract(Entity& object, ObjectData& objectData) {
	if (object.entityComponentHas<ComponentActorStateHolder>()) objectData.dataSet("State", object.entityComponentGet<ComponentActorStateHolder>()->actorStateHolder);

	objectData.dataSet("HasSenseSight", object.entityComponentHas<ComponentObjectVision>());
}

Teams::ThreatLevel ObjectAbstractor::objectThreatLevelAssess(EntityId entityId, EntityId objectId) {
	Entity& entity = EntityManager::entityGet(entityId);
	Entity& object = EntityManager::entityGet(objectId);

	if (!entity.entityComponentHas<ComponentTeam>()) return Teams::ThreatLevel::Neutral;
	if (!object.entityComponentHas<ComponentTeam>()) return Teams::ThreatLevel::Neutral;

	Teams::TeamId entityTeamId = entity.entityComponentGet<ComponentTeam>()->teamId;
	Teams::TeamId objectTeamId = object.entityComponentGet<ComponentTeam>()->teamId;

	if (entityTeamId == objectTeamId) return Teams::ThreatLevel::Ally;

	Teams::TeamRelationValue teamRelation = Teams::TeamRelationHolder::teamRelationGet(entityTeamId, objectTeamId);

	if (teamRelation <= -50.f) return Teams::ThreatLevel::Enemy;
	if (teamRelation >= +50.f) return Teams::ThreatLevel::Ally;

	return Teams::ThreatLevel::Neutral;
}
