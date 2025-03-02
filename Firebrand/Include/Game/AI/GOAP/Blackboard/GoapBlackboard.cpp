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
}


ObjectAbstractor::ObjectDataIndex Goap::Blackboard::objectAdd(ObjectAbstractor::ObjectData& objectData) {
	objects.push_back(objectData);
	return ObjectAbstractor::ObjectDataIndex(objects.size() - 1);
}
