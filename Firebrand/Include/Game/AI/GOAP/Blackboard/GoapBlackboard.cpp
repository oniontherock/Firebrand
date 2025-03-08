#include "GoapBlackboard.hpp"

Goap::Blackboard::Blackboard() {

	using namespace ObjectAbstractor;

	dataSet("Creatures", ObjectDataIndexVector());
	dataSet("Threats", ObjectDataIndexVector());
	dataSet("ThreatClosestInd", ObjectDataIndex(0));
	dataSet("ThreatClosestPolarCoordinates", sf::Vector2f(99999999.f, 0.f)); // the polar coordinates to the closest threat, we use polar coordinates so it's easier to get the threat distance
	dataSet("ThreatCount", uint32_t(0));
	dataSet("Allies", ObjectDataIndexVector());
	dataSet("Items", ObjectDataIndexVector());
	dataSet("Obstacles", ObjectDataIndexVector());

	for (uint32_t i = 0; i < 10000; i++) {
		availableObjectIds.insert(i);
	}
}


ObjectAbstractor::ObjectDataIndex Goap::Blackboard::objectAdd(ObjectAbstractor::ObjectData& objectData) {
	if (availableObjectIds.size() <= 0) {
		ConsoleHandler::consolePrintErr("Object addition attempted on blackboard when there are no more valid ids");
		return 0;
	}

	ObjectAbstractor::ObjectDataIndex ind = *availableObjectIds.begin();
	availableObjectIds.erase(ind);

	objects.insert(objects.begin() + ind, objectData);
	objectIds.insert({ objectData.dataGet<EntityId>("ObjectId"), ind });

	return ind;
}

void Goap::Blackboard::objectRemove(ObjectAbstractor::ObjectDataIndex ind) {
	if (availableObjectIds.contains(ind)) {
		ConsoleHandler::consolePrintErr("Object removal attempted on non-existant object (id=" + std::to_string(ind) + ") on Blackboard");
		return;
	}
	
	availableObjectIds.insert(ind);

	objects[ind].dataClear();
	objectIds.erase(objects[ind].dataGet<EntityId>("ObjectId"));
}

bool Goap::Blackboard::objectHas(EntityId objectId) const {
	return objectIds.contains(objectId);
}

void Goap::Blackboard::objectUpdate(ObjectAbstractor::ObjectData& objectData) {
	objects[objectIds[objectData.dataGet<EntityId>("ObjectId")]] = objectData;
}
