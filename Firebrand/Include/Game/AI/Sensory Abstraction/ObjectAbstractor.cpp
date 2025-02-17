#include "ObjectAbstractor.hpp"


DataCache ObjectAbstractor::objectDataAbstract(EntityId objectId, ObjectType objectType) {
	using namespace EntityComponents;

	Entity& object = EntityManager::entityGet(objectId);

	DataCache objectData;

	if (object.entityComponentHas<ComponentPosition>()) objectData.dataSet("Position", object.entityComponentGet<ComponentPosition>()->position);
	if (object.entityComponentHas<ComponentRotation>()) objectData.dataSet("Rotation", object.entityComponentGet<ComponentRotation>()->rotation);
	if (object.entityComponentHas<ComponentActorStateHolder>()) objectData.dataSet("State", object.entityComponentGet<ComponentActorStateHolder>()->actorStateHolder);
	if (object.entityComponentHas<ComponentObjectVision>()) objectData.dataSet("HasSenseSight", object.entityComponentHas<ComponentObjectVision>());
	objectData.dataSet("IsThreat", true); // placeholder, will just be team affiliation later, whether an object is a threat or not is more complicated, and probably needs its own function

	return objectData;
}

