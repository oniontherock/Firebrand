#include "StructurePlacer.hpp"
#include "Grid/StructurePlacementGrid.hpp"
#include "../ACECS/RNGPoolIdRegistry.hpp"
#include "../Structure Generation/Grid/StructureGridTypes.hpp"

std::vector<StructureRect> StructurePlacer::structureRectsGenerate(const PathGenerator& pathGenerator, GameLevel* level) {
	
	std::vector<StructureRect> structureRects;

	StructurePlacementGrid structurePlacementGrid(level->levelSize.x / structureGridCellSize, level->levelSize.y / structureGridCellSize, structureGridCellSize, structureGridCellSize);

	PathAxisGrid& pathAxisGrid = level->pathAxisGrid;

	constexpr float distMin = 128.f;
	constexpr float distMax = 512.f;

	for (uint16_t x = 0; x < pathAxisGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < pathAxisGrid.gridGetSizeY(); y++) {

			PathAxis& cell = pathAxisGrid.cellGet(x, y);

			if (cell.dist < distMin || cell.dist > distMax) continue;

			sf::Vector2f cellAxis = cell.dir * cell.dist;

			StructureRect rect;
			rect.width = RNGu16::getRange(5, 24) * structureGridCellSize;
			rect.height = RNGu16::getRange(5, 24) * structureGridCellSize;
			 
			rect.left = (x * structureGridCellSize) - rect.width;
			rect.top = (y * structureGridCellSize) - (rect.height / 2.f);

			rect.rotation = atan2(cellAxis.y, cellAxis.x);

			if (!(
				pathAxisGrid.worldPosIsInGridFull(uint32_t(rect.left), uint32_t(rect.top)) &&
				pathAxisGrid.worldPosIsInGridFull(uint32_t(rect.left + rect.width), uint32_t(rect.top)) &&
				pathAxisGrid.worldPosIsInGridFull(uint32_t(rect.left + rect.width), uint32_t(rect.top + rect.height)) &&
				pathAxisGrid.worldPosIsInGridFull(uint32_t(rect.left), uint32_t(rect.top + rect.height))
				)) {
				continue;
			}

			// check top face
			for (uint16_t xOffset = 0; xOffset < rect.width; xOffset++) {
				if (pathAxisGrid.cellGetFromWorld(rect.left + xOffset, rect.top).dist < distMin) {
					rect.top += structureGridCellSize;
					rect.height -= structureGridCellSize;
					xOffset = 0;

					if (rect.height <= structureGridCellSize) {
						break;
					}
				}
			}
			// check left face
			for (uint16_t yOffset = 0; yOffset < rect.height; yOffset++) {
				if (pathAxisGrid.cellGetFromWorld(rect.left, rect.top + yOffset).dist < distMin) {
					rect.left += structureGridCellSize;
					rect.width -= structureGridCellSize;
					yOffset = 0;

					if (rect.width <= structureGridCellSize) {
						break;
					}
				}
			}
			// check bottom face
			for (uint16_t xOffset = 0; xOffset < rect.width; xOffset++) {
				if (pathAxisGrid.cellGetFromWorld(rect.left + xOffset, rect.top + rect.height).dist < distMin) {
					rect.height -= structureGridCellSize;
					xOffset = 0;

					if (rect.height <= structureGridCellSize) {
						break;
					}
				}
			}

			if ((rect.width < (8.f * structureGridCellSize)) || (rect.height < (8.f * structureGridCellSize))) continue;

			sf::Vector2f structureCenter = sf::Vector2f(rect.left + (rect.width / 2.f), rect.top + rect.height / 2.f);

			// check if there is already a structure here
			bool rectIsValid = true;
			for (float xOffset = 0; xOffset < rect.width; xOffset++) {
				for (float yOffset = 0; yOffset < rect.height; yOffset++) {

					sf::Vector2f posRotated = Vector2fMath::rotate(
						(rect.left + xOffset) - structureCenter.x,
						(rect.top + yOffset) - structureCenter.y,
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
			for (float xOffset = 0; xOffset < rect.width; xOffset++) {
				for (float yOffset = 0; yOffset < rect.height; yOffset++) {

					sf::Vector2f posRotated = Vector2fMath::rotate(
						(rect.left + xOffset) - structureCenter.x,
						(rect.top + yOffset) - structureCenter.y,
						rect.rotation) + structureCenter;

					structurePlacementGrid.cellSetFromWorld(posRotated.x, posRotated.y, true);
				}
			}

			structureRects.push_back(rect);
		}
	}

	return structureRects;
}
