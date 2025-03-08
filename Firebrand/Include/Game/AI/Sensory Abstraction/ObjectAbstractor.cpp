#include "../ACECS/ECSRegistry.hpp"
#include "../GOAP/Actors/GoapActor.hpp"
#include "ObjectAbstractor.hpp"

using namespace EntityComponents;

void ObjectAbstractor::blackboardAddObjectData(Goap::Blackboard& blackboard, Entity& entity, Entity& object, ObjectType objectType) {

	ObjectData objectData = ObjectAbstractor::objectBasicDataAbstract(blackboard, entity, object, objectType);

	objectData.dataSet("TimeSeen", TimeHandler::timeSimulatedGet());

	ObjectDataIndex objectInd = blackboard.objectAdd(objectData);

	if (objectData.dataGet<bool>("IsAnimate")) {

		animateObjectDataAbstract(object, objectData);

		creatureAffiliationAbstract(blackboard, entity, Teams::ThreatLevel::Enemy, objectData, objectInd); // for now every creature is considered an enemy, this is just for testing
		//creatureAffiliationAbstract(blackboard, ObjectAbstractor::objectThreatLevelAssess(entity.Id, object.Id), objectData, objectInd);
	}
}

ObjectAbstractor::ObjectData ObjectAbstractor::objectBasicDataAbstract(Goap::Blackboard&, Entity&, Entity& object, ObjectType objectType) {

	ObjectData objectData;

	objectData.dataSet("ObjectType", objectType);
	objectData.dataSet("ObjectId", object.Id);

	if (object.entityComponentHas<ComponentRotation>()) objectData.dataSet("Rotation",  object.entityComponentGet<ComponentRotation>()->rotation);

	bool objectIsAnimate = object.entityComponentGet<ComponentIsAnimate>();
	objectData.dataSet("IsAnimate", objectIsAnimate);

	objectPositionDataAbstract(objectData, object);

	if (objectIsAnimate) {
		animateObjectDataAbstract(object, objectData);
	}

	return objectData;
}
void ObjectAbstractor::objectPositionDataAbstract(ObjectData& objectData, Entity& object) {
	objectData.dataSet("Position", object.entityComponentGet<ComponentPosition>()->position);
}
void ObjectAbstractor::animateObjectDataAbstract(Entity& object, ObjectData& objectData) {
	if (object.entityComponentHas<ComponentActorStateHolder>()) objectData.dataSet("State", object.entityComponentGet<ComponentActorStateHolder>()->actorStateHolder);

	objectData.dataSet("HasSenseSight", object.entityComponentHas<ComponentObjectVision>());
}
void ObjectAbstractor::creatureAffiliationAbstract(Goap::Blackboard& blackboard, Entity& entity, Teams::ThreatLevel threatLevel, ObjectData& objectData, ObjectDataIndex objectInd) {
	blackboard.creatures.insert(objectInd);

	switch (threatLevel) {
	case Teams::ThreatLevel::Enemy:
		blackboard.threats.insert(objectInd);
		break;
	case Teams::ThreatLevel::Ally:
		blackboard.allies.insert(objectInd);
		break;
	}
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