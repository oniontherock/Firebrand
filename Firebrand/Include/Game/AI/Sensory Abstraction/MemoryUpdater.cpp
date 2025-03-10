#include "../ACECS/ECSRegistry.hpp"
#include "../GOAP/Actors/GoapActor.hpp"
#include "../include/Common/DataCache.hpp"
#include "../include/Game/World/Objects/ObjectRegistry.hpp"
#include "../include/Game/World/Objects/ObjectTypes.hpp"
#include "../Teams/TeamRelationHolder.hpp"
#include "MemoryUpdater.hpp"

void MemoryUpdater::memoryUpdate(Goap::Blackboard& memory, ObjectAbstractor::ObjectDataVector memoryDataNew) {

	// update objects already present in memory that are to be memorized again
	for (auto& objectCurPair : memory.objects) {
		auto& objectCur = objectCurPair.second;
		for (int32_t objectMemoryCurInd = memoryDataNew.size() - 1; objectMemoryCurInd >= 0; objectMemoryCurInd--) {

			auto& objectMemoryCur = memoryDataNew[objectMemoryCurInd];

			if (objectCur.objectId == objectMemoryCur.objectId) {
				objectCur = objectMemoryCur;
				memoryDataNew.erase(memoryDataNew.begin() + objectMemoryCurInd);
			}
		}
	}
	// remove old creatures that we no longer are certain about
	for (auto& creatureIndexCur : memory.creatures) {

		ObjectData& creatureCur = memory.objects[creatureIndexCur];

		float creatureCertaintyCur = objectGetCertainty(creatureCur);

		if (creatureCertaintyCur <= 1.f) {
			memory.objectRemove(creatureIndexCur);
		}
	}
	// add new objects from the new memory data (A.K.A. memoryDataNew)
	for (auto& objectCur : memoryDataNew) {
		memory.objectAdd(objectCur);
	}

	// update object designations
	objectDesignationsUpdate(memory);
}
void MemoryUpdater::objectDesignationsUpdate(Goap::Blackboard& memory) {

	memory.objectDesignationsClear();

	for (ObjectAbstractor::ObjectDataIndex objectCurInd = 0; objectCurInd < memory.objects.size(); objectCurInd++) {

		auto& objectCur = memory.objects[objectCurInd];

		if (objectCur.isAnimate) {
			objectAnimateDesignate(memory, objectCur, objectCurInd);
		}
	}
}
void MemoryUpdater::objectAnimateDesignate(Goap::Blackboard& memory, const ObjectData& objectData, ObjectAbstractor::ObjectDataIndex objectInd) {
	memory.creatures.insert(objectInd);
	
	switch (objectData.threatLevel) {
	case Teams::ThreatLevel::Enemy:
		memory.threats.insert(objectInd);
		break;
	case Teams::ThreatLevel::Ally:
		memory.allies.insert(objectInd);
		break;
	}
}

float MemoryUpdater::objectGetCertainty(ObjectData& object) {

	// the time since the object was added to the memory in seconds
	float timeSinceMemorizedSeconds = float(TimeHandler::timeSimulatedGet() - object.timeMemorized) / 1000.f;

	// the timeSinceMemorizedSeconds is divided by this when obtaining the certainty,
	// a higher divider makes an object's certainty remain high longer
	float timeDivider = 1.f;

	// if the object is animate, it is much quicker to lose certainty about it,
	// since a living creature is likely to move, while a gun on the floor probably will remain where it is
	if (!object.isAnimate) {
		timeDivider *= 10.f;
	}

	return Mathf::clamp(100.f - (timeSinceMemorizedSeconds / timeDivider), 0.f, 100.f);
}
