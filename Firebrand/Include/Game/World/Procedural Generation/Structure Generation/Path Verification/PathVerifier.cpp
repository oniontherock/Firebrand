#include "../Include/Game/Pathfinding/AStar/Path Creation/AStarPathfinder.hpp"
#include "PathVerifier.hpp"


bool PathVerifier::pointsConnect(const WallGrid2D& wallGrid, sf::Vector2u structureSize, sf::Vector2u pointA, sf::Vector2u pointB) {

	AStarGrid aStarGrid = AStarGrid(structureSize.x, structureSize.y, structureGridCellSize, structureGridCellSize);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			aStarGrid.cellGet(x, y).valid = wallGrid[x][y];
		}
	}

	AStarPath path = AStarPathfinder::pathGetFromCellCoordinates(pointA, pointB, aStarGrid);

	// if the end point of the path is not equal to pointB, that means we couldn't find a path to the point, so return false
	if (Vector2fMath::distSqrd(path[path.size() - 1], sf::Vector2f(pointB)) > 1.f * 1.f) {
		return false;
	}

	return true;
}

