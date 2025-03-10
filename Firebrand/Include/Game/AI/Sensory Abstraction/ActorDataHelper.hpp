#ifndef __ACTOR_DATA_HELPER_H__
#define __ACTOR_DATA_HELPER_H__

#include "../GOAP/Actors/GoapActor.hpp"

namespace Goap {
	// namespace that is primarily used by actions for things like getting the closest threat to a point
	namespace ActorDataHelper {

		sf::Vector2f pointGetClosestThreatPolarCoordinates(Actor& actor, sf::Vector2f point);
	}
}


#endif