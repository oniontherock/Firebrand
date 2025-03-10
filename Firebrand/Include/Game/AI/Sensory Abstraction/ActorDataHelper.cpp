#include "ActorDataHelper.hpp"
#include <Auxiliary/VectorMath.hpp>

sf::Vector2f Goap::ActorDataHelper::pointGetClosestThreatPolarCoordinates(Actor& actor, sf::Vector2f point) {
	auto& blackboard = actor.blackboard;

	sf::Vector2f closestThreatPolarCoordinates = sf::Vector2f(9999999999999999.f, 0);

	for (ObjectAbstractor::ObjectDataIndex i : blackboard.threats) {
		ObjectData& threatCur = blackboard.objects[i];

		sf::Vector2f axis = threatCur.position - point;

		float axisLenSqrd = Vector2fMath::lengthSqrd(axis);

		// note that closestThreatPolarCoordinates.x is also squared here, the square root is obtained later
		if (axisLenSqrd < closestThreatPolarCoordinates.x) {
			closestThreatPolarCoordinates.x = axisLenSqrd;
			closestThreatPolarCoordinates.y = atan2(axis.y, axis.x);
		}
	}

	closestThreatPolarCoordinates.x = sqrt(closestThreatPolarCoordinates.x);

	return closestThreatPolarCoordinates;
}

