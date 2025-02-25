#include "GoapBlackboard.hpp"

ObjectDataIndex Goap::Blackboard::objectAdd(ObjectData& objectData) {
	objects.push_back(objectData);
	return objects.size() - 1;
}
