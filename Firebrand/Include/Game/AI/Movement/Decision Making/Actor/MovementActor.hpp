#ifndef __MOVEMENT_ACTOR_H__
#define __MOVEMENT_ACTOR_H__

#include "../Movement Points/MovementPlanner.hpp"
#include "../Movement Points/MovementPointHandler.hpp"

namespace Movement {
	struct Actor {
		MovementPointHandler movementPointHandler;
	};
}

#endif