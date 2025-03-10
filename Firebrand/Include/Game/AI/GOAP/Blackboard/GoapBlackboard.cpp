#include "GoapBlackboard.hpp"

Goap::Blackboard::Blackboard() {
	for (uint32_t i = 0; i < 10000; i++) {
		availableObjectIds.insert(i);
	}
}

const DataUMap& Goap::Blackboard::whiteDataMapGet() {
	return whiteData.dataUMapGet();
}

void Goap::Blackboard::objectDesignationsClear() {
	creatures.clear();
	threats.clear();
	allies.clear();
	items.clear();
	obstacles.clear();
}

ObjectAbstractor::ObjectDataIndex Goap::Blackboard::objectAdd(ObjectData& objectData) {
	if (availableObjectIds.size() <= 0) {
		ConsoleHandler::consolePrintErr("Object addition attempted on blackboard when there are no more valid ids");
		return 0;
	}

	ObjectAbstractor::ObjectDataIndex ind = *availableObjectIds.begin();
	availableObjectIds.erase(ind);

	objects.insert({ ind, objectData });
	objectIds.insert({ objectData.objectId, ind });

	return ind;
}

void Goap::Blackboard::objectRemove(ObjectAbstractor::ObjectDataIndex ind) {
	if (availableObjectIds.contains(ind)) {
		ConsoleHandler::consolePrintErr("Object removal attempted on non-existant object (ind=" + std::to_string(ind) + ") on Blackboard");
		return;
	}
	
	availableObjectIds.insert(ind);

	objectIds.erase(objects[ind].objectId);
	objects.erase(ind);
}

bool Goap::Blackboard::objectHas(EntityId objectId) const {
	return objectIds.contains(objectId);
}

void Goap::Blackboard::objectUpdate(ObjectData& objectData) {
	objects[objectIds[objectData.objectId]] = objectData;
}
