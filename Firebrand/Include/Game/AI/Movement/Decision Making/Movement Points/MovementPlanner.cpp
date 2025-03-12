#include "MovementPlanner.hpp"
#include "../include/Game/Pathfinding/AStar/Path Creation/PathRequestManager.hpp"
#include "../ACECS/ECSRegistry.hpp"

void Movement::MovementPlanner::movementsPlan(Entity& entity, MovementPointHandler movementPointHandler) {

	AStarPathfinder::SpecializationFunction function = [movementPointHandler](AStarCell& cell) {

		AStarCostValue costValue = 0;

		for (const MovementPoint& movementPointCur : movementPointHandler.avoidancePoints) {
			float distSqrd = Vector2fMath::distSqrd(movementPointCur, cell.cellPositionWorld);

			if (distSqrd > movementPointCur.threshold * movementPointCur.threshold) continue;

			float dist = sqrt(distSqrd);

			costValue += AStarCostValue(movementPointCur.threshold - dist) * movementPointCur.priority;
		}

		return costValue;
		};

	sf::Vector2f startPos = entity.entityComponentGet<EntityComponents::ComponentPosition>()->position;
	sf::Vector2f endPos = movementPointHandler.targetPoints[0];

	AStarGrid& aStarGrid = GameLevelGrid::levelGet(entity.levelId)->aStarGrid;

	PathRequestManager::pathRequest(startPos, endPos, entity.Id, function);

}
