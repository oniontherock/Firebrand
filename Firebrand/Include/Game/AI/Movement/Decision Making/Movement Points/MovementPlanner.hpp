#ifndef __MOVEMENT_PLANNER_H__
#define __MOVEMENT_PLANNER_H__

#include "MovementPointHandler.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "../include/Game/Pathfinding/AStar/Path Creation/PathRequestManager.hpp"

namespace Movement {
	namespace MovementPlanner {
		void movementPlan(Entity& entity, MovementPointHandler movementPointHandler);
	};
}

#endif