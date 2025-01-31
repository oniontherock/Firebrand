#include "PathAxisGenerator.hpp"
#include <Auxiliary/VectorMath.hpp>

sf::Vector2f PathAxisGenerator::pointGetAxisToPathPointClosest(std::vector<sf::Vector2f> pathPoints, sf::Vector2f point) {
	sf::Vector2f axisBest;
	float distSqrdBest = 99999999;

	for (const sf::Vector2f& pointCur : pathPoints) {
		sf::Vector2f axisCur = Vector2fMath::axis(pointCur, point);
		float distSqrdCur = Vector2fMath::lengthSqrd(axisCur);

		if (distSqrdCur > distSqrdBest) {
			axisBest = axisCur;
			distSqrdBest = distSqrdCur;
		}
	}

	return axisBest;
}


PathAxisGrid PathAxisGenerator::pathAxisGridGenerate(std::vector<sf::Vector2f> pathPoints, GameLevel* level) {

	constexpr float cellSize = 64.f;

	PathAxisGrid pathDistanceGrid(level->levelSize.x / cellSize, level->levelSize.y / cellSize, cellSize, cellSize);

	for (uint16_t x = 0; x < pathDistanceGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < pathDistanceGrid.gridGetSizeY(); y++) {
			sf::Vector2f cellWorldPos = pathDistanceGrid.coordinatesCellToWorld(x, y);

			sf::Vector2f axis = pointGetAxisToPathPointClosest(pathPoints, cellWorldPos);

			float axisLen = Vector2fMath::length(axis);

			PathAxis pathAxis(axis / axisLen, axisLen);

			pathDistanceGrid.cellSet(x, y, pathAxis);
		}
	}

	return PathAxisGrid(1, 1, 1, 1);
}

