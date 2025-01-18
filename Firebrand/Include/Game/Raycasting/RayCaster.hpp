#ifndef __RAY_CASTER_H__
#define __RAY_CASTER_H__

#include <cstdint>
#include <World/LevelTypeDefinitions.hpp>
#include <SFML/System/Vector2.hpp>
#include "Occlusion Grid/OcclusionGrid.hpp"

// casts a series of rays out from a position
struct RayCaster {

	RayCaster() = default;
	RayCaster(OcclusionGrid* _occlusionGrid);


	// update the RayCaster, always calls raysCast().
	virtual void update(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount);

	// OcclusionGrid used by this RayCaster for occlusion, if set to nullptr, does not occlude
	OcclusionGrid* occlusionGrid = nullptr;
protected:
	
	// casts out a series of rays from castPosition towards an castAngle in a cone
	//
	// @param castPosition: the position to cast from.
	// @param castAngle: the angle at the center of the cone.
	// @param coneSize: the angular size of the cone to cast rays in,
	// if castAngle is 0, and coneSize is 90, then the most clockwise ray is 45, and the most counterclockwise is -45.
	// @param rayMaxDist: the maximum dist a ray can travel.
	// @param rayCount: the amount of rays to cast.
	void raysCast(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount);

	// performs some operation on a ray each time it steps,
	// can perform different operations from child to child,
	// returns a bool, if false, the ray stops stepping and the next ray is launched
	virtual bool rayUpdate(sf::Vector2f rayPosition) = 0;
};



#endif