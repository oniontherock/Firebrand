#include "GoapBlackboard.hpp"

Goap::Blackboard::Blackboard() {

	whiteData.dataSet("ThreatClosestInd", ObjectAbstractor::ObjectDataIndexVector());
	whiteData.dataSet("ThreatClosestPolarCoordinates", sf::Vector2f(99999999.f, 0.f));
	whiteData.dataSet("ThreatCount", uint32_t(0));

	for (uint32_t i = 0; i < 10000; i++) {
		availableObjectIds.insert(i);
	}
}

const DataUMap& Goap::Blackboard::whiteDataMapGet() {
	return whiteData.dataUMapGet();
}

ObjectAbstractor::ObjectDataIndex Goap::Blackboard::objectAdd(ObjectData& objectData) {
	if (availableObjectIds.size() <= 0) {
		ConsoleHandler::consolePrintErr("Object addition attempted on blackboard when there are no more valid ids");
		return 0;
	}

	ObjectAbstractor::ObjectDataIndex ind = *availableObjectIds.begin();
	availableObjectIds.erase(ind);

	objects.insert(objects.begin() + ind, objectData);
	objectIds.insert({ objectData.objectId, ind });

	return ind;
}

void Goap::Blackboard::objectRemove(ObjectAbstractor::ObjectDataIndex ind) {
	if (availableObjectIds.contains(ind)) {
		ConsoleHandler::consolePrintErr("Object removal attempted on non-existant object (id=" + std::to_string(ind) + ") on Blackboard");
		return;
	}
	
	availableObjectIds.insert(ind);

	objects[ind].~ObjectData();
	objectIds.erase(objects[ind].objectId);
}

bool Goap::Blackboard::objectHas(EntityId objectId) const {
	return objectIds.contains(objectId);
}

void Goap::Blackboard::objectUpdate(ObjectData& objectData) {
	objects[objectIds[objectData.objectId]] = objectData;
}
