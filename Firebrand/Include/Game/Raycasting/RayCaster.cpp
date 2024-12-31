#include "RayCaster.hpp"
#include <cmath>

void RayCaster::update(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount) {
	raysCast(castPosition, castAngle, coneSize, rayMaxDist, rayCount);
}

void RayCaster::raysCast(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount) {

	// adjust cast angle so that it is at the center of the cone
	castAngle -= coneSize / 2.f;

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / rayCount;

	for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = castAngle + (float(curRayInd) * rayAngleDifference);
		// heading of the ray
		const sf::Vector2f rayHeading = sf::Vector2f(cos(rayRotation), sin(rayRotation));
		// iterate from 0.f to rayMaxDist in steps of 1.f, updating the ray each iteration
		for (float dist = 0.f; dist < rayMaxDist; dist += 1.f) {
			// call ray update with the position of castPosition with the heading added multiplied by the dist, which is the rays position,
			// breaks if ray update returns false
			if (!rayUpdate(castPosition + (rayHeading * dist))) break;
		}
	}
}
