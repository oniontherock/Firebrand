#include "StructureGenerator.hpp"
#include <Auxiliary/Math.hpp>




StructureGrid StructureGenerator::structureGenerate(StructureType structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2i structureSize) {

	std::vector<std::vector<bool>> wallGrid = std::vector<std::vector<bool>>(structureSize.x, std::vector<bool>(structureSize.y, false));

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			if ((x == 0 || x == structureSize.x - 1) || (y == 0 || y == structureSize.y - 1)) {
				wallGrid[x][y] = true;
			}
		}
	}

	StructureGrid structureGrid = StructureGrid(structureSize.x, structureSize.y, structureType, structurePosition, structureRotation);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			if (!wallGrid[x][y]) continue;

			std::vector<bool> wallStates(9, false);

			for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
				for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

					// if the offset is even it means it's either a diagonal, or (0, 0), which we don't want, so skip if it is
					if (Mathi16::isEven(abs(offsetX + offsetY))) continue;

					int16_t offsettedX = x + offsetX;
					int16_t offsettedY = y + offsetY;

					if (!structureGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

					uint16_t flattenedIndex = uint16_t(((offsetY + 1) * 3) + (offsetX + 1));

					wallStates[flattenedIndex] = wallGrid[offsettedX][offsettedY];
				}
			}

			std::pair<WallGenerator::WallType, float> wallData = WallGenerator::wallDataGetFromSurroundings(wallStates);

			StructureCell cell = StructureCell(WallGenerator::cellTypeGetFromWallType(wallData.first), wallData.second);

			structureGrid.cellSet(x, y, cell);
		}
	}

	return structureGrid;
}

