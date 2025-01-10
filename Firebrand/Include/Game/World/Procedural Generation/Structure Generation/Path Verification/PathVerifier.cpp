#include "../Include/Game/Pathfinding/AStar/Path Creation/AStarPathfinder.hpp"
#include "PathVerifier.hpp"


bool PathVerifier::pointsConnect(const WallGrid2D& wallGrid, sf::Vector2u structureSize, sf::Vector2u pointA, sf::Vector2u pointB) {

	AStarGrid aStarGrid = AStarGrid(structureSize.x, structureSize.y, 1, 1);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			aStarGrid.cellGet(x, y).valid = !wallGrid[x][y];
		}
	}

	AStarPath path = AStarPathfinder::pathGet(sf::Vector2f(pointA), sf::Vector2f(pointB), aStarGrid);

	// return whether a path was found
	return path.size() > 0;
}

