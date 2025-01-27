#include "StructurePlacer.hpp"
#include "Grid/StructurePlacementGrid.hpp"
#include "../ACECS/RNGPoolIdRegistry.hpp"
#include "../Structure Generation/Grid/StructureGridTypes.hpp"

std::vector<StructureRect> StructurePlacer::structureRectsGenerate(const PathGenerator& pathGenerator, GameLevel* level) {
	
	std::vector<StructureRect> structureRects;
	
	const std::vector<PathPoint*>& path = pathGenerator.pathGet();
	const std::vector<PointConnection>& connections = pathGenerator.connectionsGet();

	constexpr float cellSize = 64;

	StructurePlacementGrid structurePlacementGrid = StructurePlacementGrid(level->levelSize.x / uint16_t(cellSize), level->levelSize.y / uint16_t(cellSize), cellSize, cellSize);

	// mark paths as being structures in the structurePlacementGrid, this is done so we don't place structures on the paths
	for (const PointConnection& connectionCur : connections) {
		PathPoint* pointA = path[connectionCur.first];
		PathPoint* pointB = path[connectionCur.second];

		sf::Vector2f point = pointA->position;

		sf::Vector2f axis = Vector2fMath::dir(pointA->position, pointB->position) * cellSize;

		while (Vector2fMath::distSqrd(point, pointB->position) > (cellSize * cellSize)) {


			for (int16_t offsetX = -2; offsetX <= 2; offsetX++) {
				for (int16_t offsetY = -2; offsetY <= 2; offsetY++) {

					sf::Vector2f offsettedPoint = point + sf::Vector2f(float(offsetX) * cellSize, float(offsetY) * cellSize);

					if (!structurePlacementGrid.worldPosIsInGridFull(offsettedPoint)) continue;

					structurePlacementGrid.cellSetFromWorld(offsettedPoint, true);
				}
			}

			point += axis;
		}
	}

	for (const PathPoint* pointCur : path) {
		while (RNGf::probability(1.f)) {

			sf::Vector2f point = pointCur->position;

			float randAngle = RNGf::getRange(Mathf::TAU);

			sf::Vector2f placementAxis = sf::Vector2f(cos(randAngle), sin(randAngle));


			StructureRect structureRect;
			structureRect.width = RNGu16::getRange(8, 24) * structureGridCellSize;
			structureRect.height = RNGu16::getRange(8, 24) * structureGridCellSize;

			float maxDist = std::max(structureRect.width, structureRect.height);

			placementAxis *= maxDist;

			structureRect.left = (point.x + placementAxis.x) - (structureRect.width / 2.f);
			structureRect.top = (point.y + placementAxis.y) - (structureRect.height / 2.f);


			bool rectIsClear = true;
			for (float x = 0; x < structureRect.width; x += cellSize) {
				for (float y = 0; y < structureRect.height; y += cellSize) {

					float worldX = structureRect.left + x;
					float worldY = structureRect.top + y;

					if (!structurePlacementGrid.worldPosIsInGridFull(worldX, worldY)) {
						rectIsClear = false;
						goto clearLoopEnd;
					}

					if (structurePlacementGrid.cellGetFromWorld(worldX, worldY)) {
						rectIsClear = false;
						goto clearLoopEnd;
					}

				}
			}
		clearLoopEnd:

			if (rectIsClear) {

				for (float x = 0; x < structureRect.width; x += cellSize) {
					for (float y = 0; y < structureRect.height; y += cellSize) {

						float worldX = structureRect.left + x;
						float worldY = structureRect.top + y;

						structurePlacementGrid.cellSetFromWorld(worldX, worldY, true);
					}
				}

				structureRect.rotation = atan2(-placementAxis.y, -placementAxis.x);

				structureRects.push_back(structureRect);
			}
		}
	}

	return structureRects;
}

