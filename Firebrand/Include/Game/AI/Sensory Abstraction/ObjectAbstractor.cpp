#include "../ACECS/ECSRegistry.hpp"
#include "../GOAP/Actors/GoapActor.hpp"
#include "ObjectAbstractor.hpp"

using namespace EntityComponents;

ObjectData ObjectAbstractor::blackboardAddObjectData(Entity& entity, Entity& object, ObjectType objectType) {

	ObjectData objectData = ObjectAbstractor::objectBasicDataAbstract(entity, object, objectType);

	objectData.timeMemorized = TimeHandler::timeSimulatedGet();

	if (objectData.isAnimate) {

		animateObjectDataAbstract(object, objectData);

		objectData.threatLevel = Teams::ThreatLevel::Enemy; // for now every creature is considered an enemy, this is just for testing
		//objectData.threatLevel = ObjectAbstractor::objectThreatLevelAssess(entity.Id, object.Id);
	}

	return objectData;
}

ObjectData ObjectAbstractor::objectBasicDataAbstract(Entity&, Entity& object, ObjectType objectType) {

	ObjectData objectData;

	objectData.objectType = objectType;
	objectData.objectId = object.Id;

	if (object.entityComponentHas<ComponentRotation>()) objectData.rotation = object.entityComponentGet<ComponentRotation>()->rotation;

	bool objectIsAnimate = object.entityComponentGet<ComponentIsAnimate>();
	objectData.isAnimate = objectIsAnimate;

	objectPositionDataAbstract(objectData, object);

	if (objectIsAnimate) {
		animateObjectDataAbstract(object, objectData);
	}

	return objectData;
}
void ObjectAbstractor::objectPositionDataAbstract(ObjectData& objectData, Entity& object) {
	objectData.position = object.entityComponentGet<ComponentPosition>()->position;
}
void ObjectAbstractor::animateObjectDataAbstract(Entity& object, ObjectData& objectData) {
	if (object.entityComponentHas<ComponentActorStateHolder>()) objectData.state = object.entityComponentGet<ComponentActorStateHolder>()->actorStateHolder;

	objectData.hasSenseSight = object.entityComponentHas<ComponentObjectVision>();
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