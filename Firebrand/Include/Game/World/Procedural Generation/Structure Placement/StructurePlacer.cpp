#include "StructurePlacer.hpp"
#include "Grid/StructurePlacementGrid.hpp"
#include "../ACECS/RNGPoolIdRegistry.hpp"
#include "../Structure Generation/Grid/StructureGridTypes.hpp"

std::vector<StructureRect> StructurePlacer::structureRectsGenerate(GameLevel* level) {
	
	std::vector<StructureRect> structureRects;

	StructurePlacementGrid structurePlacementGrid(uint32_t(level->levelSize.x / structureGridCellSize), uint32_t(level->levelSize.y / structureGridCellSize), structureGridCellSize, structureGridCellSize);

	PathAxisGrid& pathAxisGrid = level->pathAxisGrid;

	constexpr float distMin = 128.f;
	constexpr float distMax = 512.f;

	for (uint16_t x = 0; x < pathAxisGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < pathAxisGrid.gridGetSizeY(); y++) {

			PathAxis& cell = pathAxisGrid.cellGet(x, y);

			if (cell.dist < distMin || cell.dist > distMax) continue;

			sf::Vector2f cellAxis = cell.dir * cell.dist;

			StructureRect rect;
			rect.size.x = RNGu16::getRange(5, 24) * structureGridCellSize;
			rect.size.y = RNGu16::getRange(5, 24) * structureGridCellSize;
			 
			rect.position.x = (x * structureGridCellSize) - rect.size.x;
			rect.position.y = (y * structureGridCellSize) - (rect.size.y / 2.f);

			rect.rotation = atan2(cellAxis.y, cellAxis.x);

			if (!(
				pathAxisGrid.worldPosIsInGridFull(rect.position.x, rect.position.y) &&
				pathAxisGrid.worldPosIsInGridFull(rect.position.x + rect.size.x, rect.position.y) &&
				pathAxisGrid.worldPosIsInGridFull(rect.position.x + rect.size.x, rect.position.y + rect.size.y) &&
				pathAxisGrid.worldPosIsInGridFull(rect.position.x, rect.position.y + rect.size.y)
				)) {
				continue;
			}

			// check position.y face
			for (uint16_t xOffset = 0; xOffset < rect.size.x; xOffset++) {
				if (pathAxisGrid.cellGetFromWorld(rect.position.x + xOffset, rect.position.y).dist < distMin) {
					rect.position.y += structureGridCellSize;
					rect.size.y -= structureGridCellSize;
					xOffset = 0;

					if (rect.size.y <= structureGridCellSize) {
						break;
					}
				}
			}
			// check position.x face
			for (uint16_t yOffset = 0; yOffset < rect.size.y; yOffset++) {
				if (pathAxisGrid.cellGetFromWorld(rect.position.x, rect.position.y + yOffset).dist < distMin) {
					rect.position.x += structureGridCellSize;
					rect.size.x -= structureGridCellSize;
					yOffset = 0;

					if (rect.size.x <= structureGridCellSize) {
						break;
					}
				}
			}
			// check bottom face
			for (uint16_t xOffset = 0; xOffset < rect.size.x; xOffset++) {
				if (pathAxisGrid.cellGetFromWorld(rect.position.x + xOffset, rect.position.y + rect.size.y).dist < distMin) {
					rect.size.y -= structureGridCellSize;
					xOffset = 0;

					if (rect.size.y <= structureGridCellSize) {
						break;
					}
				}
			}

			if ((rect.size.x < (8.f * structureGridCellSize)) || (rect.size.y < (8.f * structureGridCellSize))) continue;

			sf::Vector2f structureCenter = sf::Vector2f(rect.position.x + (rect.size.x / 2.f), rect.position.y + rect.size.y / 2.f);

			// check if there is already a structure here
			bool rectIsValid = true;
			for (float xOffset = 0; xOffset < rect.size.x; xOffset++) {
				for (float yOffset = 0; yOffset < rect.size.y; yOffset++) {

					sf::Vector2f posRotated = Vector2fMath::rotate(
						(rect.position.x + xOffset) - structureCenter.x,
						(rect.position.y + yOffset) - structureCenter.y,
						rect.rotation) + structureCenter;

					if (!structurePlacementGrid.worldPosIsInGridFull(posRotated.x, posRotated.y)) {
						rectIsValid = false;
						goto endPlacementCheck;
					}

					if (structurePlacementGrid.cellGetFromWorld(posRotated.x, posRotated.y)) {
						rectIsValid = false;
						goto endPlacementCheck;
					}
				}
			}
			endPlacementCheck:

			if (!rectIsValid) continue;

			// mark area as having a structure
			for (float xOffset = 0; xOffset < rect.size.x; xOffset++) {
				for (float yOffset = 0; yOffset < rect.size.y; yOffset++) {

					sf::Vector2f posRotated = Vector2fMath::rotate(
						(rect.position.x + xOffset) - structureCenter.x,
						(rect.position.y + yOffset) - structureCenter.y,
						rect.rotation) + structureCenter;

					structurePlacementGrid.cellSetFromWorld(posRotated.x, posRotated.y, true);
				}
			}

			structureRects.push_back(rect);
		}
	}

	return structureRects;
}
