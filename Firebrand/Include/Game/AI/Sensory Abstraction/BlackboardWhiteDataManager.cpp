#include "BlackboardWhiteDataManager.hpp"
#include "../GOAP/Blackboard/GoapBlackboard.hpp"
#include "ObjectAbstractor.hpp"

#include "../ACECS/ECSRegistry.hpp"

void BlackboardWhiteDataManager::whiteDataObtain(Entity& actor, Goap::Blackboard& blackboard) {

	using namespace EntityComponents;
	using namespace ObjectAbstractor;

	// increment threat count by one
	blackboard.whiteDataSet("ThreatCount", uint32_t(blackboard.threats.size()));

	sf::Vector2f threatClosestPolarCoordinates = sf::Vector2f(9999999999.f, 0.f);
	uint32_t threatClosestInd = 0;
	for (ObjectAbstractor::ObjectDataIndex objectDataIndex : blackboard.threats) {

		ObjectData& threatCur = blackboard.objects[objectDataIndex];

		// determine if the threat is the closest threat, and if so, set the closest threat related data to that of this threat
		// first get threat axis (axis from the entity to the threat)
		sf::Vector2f threatAxis = Vector2fMath::axis(actor.entityComponentGet<ComponentPosition>()->position, threatCur.position);

		// then get distance from entity to threat
		float threatDist = Vector2fMath::length(threatAxis);
		// if the distance from the entity to the threat is less (or equal to) the current closest threat's distance, then we set the closest threat data to that of the current object
		if (threatDist <= threatClosestPolarCoordinates.x) {
			sf::Vector2f threatPolarCoordinates;
			threatPolarCoordinates.x = threatDist;
			threatPolarCoordinates.y = atan2(threatAxis.y, threatAxis.x);
			threatClosestPolarCoordinates = threatPolarCoordinates;
			threatClosestInd = objectDataIndex;
		}
	}

	blackboard.whiteDataSet("ThreatClosestPolarCoordinates", threatClosestPolarCoordinates);
	blackboard.whiteDataSet("ThreatClosestInd", threatClosestInd);
}

