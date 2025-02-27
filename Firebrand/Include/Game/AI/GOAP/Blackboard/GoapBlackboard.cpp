#include "GoapBlackboard.hpp"

Goap::Blackboard::Blackboard() {

	dataSet("Creatures", ObjectDataIndexVector());
	dataSet("Threats", ObjectDataIndexVector());
	dataSet("ThreatClosestInd", 0);
	dataSet("ThreatCount", uint32_t(0));
	dataSet("Allies", ObjectDataIndexVector());
	dataSet("Items", ObjectDataIndexVector());
	dataSet("Obstacles", ObjectDataIndexVector());
}


ObjectDataIndex Goap::Blackboard::objectAdd(ObjectData& objectData) {
	objects.push_back(objectData);
	return objects.size() - 1;
}
