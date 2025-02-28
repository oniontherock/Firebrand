#include "../ACECS/ECSRegistry.hpp"
#include "../GOAP/Actors/GoapActor.hpp"
#include "ObjectAbstractor.hpp"

using namespace EntityComponents;

void ObjectAbstractor::blackboardAddObjectData(Goap::Blackboard& blackboard, Entity& entity, Entity& object, ObjectType objectType) {
	ObjectData objectData = ObjectAbstractor::objectBasicDataAbstract(blackboard, entity, object, objectType);

	ObjectDataIndex objectInd = blackboard.objectAdd(objectData);

	if (objectData.dataGet<bool>("IsAnimate")) {

		animateObjectDataAbstract(object, objectData);

		creatureAffiliationAbstract(blackboard, entity, Teams::ThreatLevel::Enemy, objectData, objectInd); // for now every creature is considered an enemy, this is just for testing
		//creatureAffiliationAbstract(blackboard, ObjectAbstractor::objectThreatLevelAssess(entity.Id, object.Id), objectData, objectInd);
	}
}

ObjectAbstractor::ObjectData ObjectAbstractor::objectBasicDataAbstract(Goap::Blackboard& blackboard, Entity& entity, Entity& object, ObjectType objectType) {

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
void ObjectAbstractor::creatureAffiliationAbstract(Goap::Blackboard& blackboard, Entity& entity, Teams::ThreatLevel threatLevel, ObjectData& objectData, ObjectDataIndex objectInd) {
	blackboard.dataGet<ObjectDataIndexVector&>("Creatures").insert(objectInd);

	switch (threatLevel) {
	case Teams::ThreatLevel::Enemy:
		enemyDataAbstract(blackboard, entity, objectData, objectInd);
		break;
	case Teams::ThreatLevel::Neutral:
		neutralDataAbstract(blackboard, entity, objectData, objectInd);
		break;
	case Teams::ThreatLevel::Ally:
		allyDataAbstract(blackboard, entity, objectData, objectInd);
		break;
	}
}

void ObjectAbstractor::enemyDataAbstract(Goap::Blackboard& blackboard, Entity& entity, ObjectData& objectData, ObjectDataIndex objectInd) {
	// add object to threat list
	blackboard.dataGet<ObjectDataIndexVector&>("Threats").insert(objectInd);
	// increment threat count by one
	blackboard.dataGet<uint32_t&>("ThreatCount") += 1;
	// determine if the threat is the closest threat, and if so, set the closest threat related data to that of this threat
	// first get threat axis (axis from the entity to the threat)
	sf::Vector2f threatAxis = Vector2fMath::axis(entity.entityComponentGet<ComponentPosition>()->position, objectData.dataGet<sf::Vector2f>("Position"));
	// then get distance from entity to threat
	float threatDist = Vector2fMath::length(threatAxis);
	// if the distance from the entity to the threat is less (or equal to) the current closest threat's distance, then we set the closest threat data to that of the current object
	if (threatDist <= blackboard.dataGet<sf::Vector2f>("ThreatClosestPolarCoordinates").x) {
		sf::Vector2f threatPolarCoordinates;
		threatPolarCoordinates.x = threatDist;
		threatPolarCoordinates.y = atan2(threatAxis.y, threatAxis.x);
		blackboard.dataSet("ThreatClosestPolarCoordinates", threatPolarCoordinates);
		blackboard.dataSet("ThreatClosestInd", objectInd);
	}
}
void ObjectAbstractor::neutralDataAbstract(Goap::Blackboard& blackboard, Entity& entity, ObjectData& objectData, ObjectDataIndex objectInd) {
}
void ObjectAbstractor::allyDataAbstract(Goap::Blackboard& blackboard, Entity& entity, ObjectData& objectData, ObjectDataIndex objectInd) {
	blackboard.dataGet<ObjectDataIndexVector&>("Allies").insert(objectInd);
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