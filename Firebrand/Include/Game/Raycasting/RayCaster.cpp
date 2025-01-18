#include "RayCaster.hpp"
#include <cmath>

RayCaster::RayCaster(OcclusionGrid* _occlusionGrid) {
	occlusionGrid = _occlusionGrid;
}


void RayCaster::update(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount) {
	raysCast(castPosition, castAngle, coneSize, rayMaxDist, rayCount);
}

void RayCaster::raysCast(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount) {

	const bool doOcclude = occlusionGrid != nullptr;

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
			// calculate ray position from castPosition and heading * dist
			sf::Vector2f rayPositionCur = castPosition + (rayHeading * dist);

			// if we should occlude, check if the ray's position falls on a cell in the occlusion grid which is marked as occluding, if that's the case, break
			if (doOcclude) if (occlusionGrid->cellGetFromWorld(rayPositionCur)) break;
			// call ray update with the position of castPosition with the heading added multiplied by the dist, which is the rays position,
			// breaks if ray update returns false
			if (!rayUpdate(rayPositionCur)) break;
		}
	}
}
