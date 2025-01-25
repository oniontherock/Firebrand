#include "StructurePlacer.hpp"
#include "Grid/StructurePlacementGrid.hpp"
#include "../ACECS/RNGPoolIdRegistry.hpp"
#include "../Structure Generation/Grid/StructureGridTypes.hpp"

std::vector<StructureRect> StructurePlacer::structureRectsGenerate(const PathGenerator& pathGenerator, GameLevel* level) {
	
	std::vector<StructureRect> structureRects;
	
	const std::vector<PathPoint*>& path = pathGenerator.pathGet();
	const std::vector<PointConnection>& connections = pathGenerator.connectionsGet();

	constexpr float cellSize = 256;

	StructurePlacementGrid structurePlacementGrid = StructurePlacementGrid(level->levelSize.x / uint16_t(cellSize), level->levelSize.y / uint16_t(cellSize), cellSize, cellSize);

	// mark paths as being structures in the structurePlacementGrid, this is done so we don't place structures on the paths
	for (const PointConnection& connectionCur : connections) {
		PathPoint* pointA = path[connectionCur.first];
		PathPoint* pointB = path[connectionCur.second];

		sf::Vector2f point = pointA->position;

		sf::Vector2f axis = Vector2fMath::dir(pointA->position, pointB->position) * cellSize;

		while (Vector2fMath::distSqrd(point, pointB->position) > (cellSize * cellSize)) {

			if (!structurePlacementGrid.worldPosIsInGridFull(point)) break;

			structurePlacementGrid.cellSetFromWorld(pointA->position, true);
			point += axis;
		}
	}

	for (const PointConnection& connectionCur : connections) {
		PathPoint* pointA = path[connectionCur.first];
		PathPoint* pointB = path[connectionCur.second];

		sf::Vector2f point = pointA->position;

		sf::Vector2f axis = Vector2fMath::dir(pointA->position, pointB->position) * cellSize;

		while (Vector2fMath::distSqrd(point, pointB->position) > (cellSize * cellSize)) {
			if (0.1f > RNGfPool::poolValueGet(PROBABILITY_POOL_ID)) {

				sf::Vector2f placementAxis = Vector2fMath::normalize(Vector2fMath::rotate(axis, RNGfPool::poolValueGet(PROBABILITY_POOL_ID) ? (Mathf::PI / 2.f) : (-Mathf::PI / 2.f)));


				StructureRect structureRect;
				structureRect.width = RNGu16::getRange(8, 16) * structureGridCellSize;
				structureRect.height = RNGu16::getRange(8, 16) * structureGridCellSize;

				float maxDist = std::max(structureRect.width, structureRect.height);

				placementAxis *= maxDist * RNGf::getRange(1.25f, 2.f);

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

					structureRects.push_back(structureRect);
				}

				break;
			}

			point += axis;
		}
	}

	return structureRects;
}

