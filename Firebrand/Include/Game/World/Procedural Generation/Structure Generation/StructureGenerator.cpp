#include "Room Designation/RoomDesignator.hpp"
#include "StructureGenerator.hpp"
#include <Auxiliary/Math.hpp>



StructureGrid StructureGenerator::structureGenerate(StructureTypeBase* structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2u structureSize) {

	WallGrid2D wallGrid;

	RoomTypeGrid roomTypeGrid = RoomTypeGrid(structureSize.x, structureSize.y);

	for (uint16_t i = 0; i < 64; i ++) {

		wallGrid = WallGenerator::wallsGenerate(structureType, structureSize);

		bool generationSucceeded = RoomDesignator::structureRoomTypesDesignate(wallGrid, roomTypeGrid, structureSize, WallGenerator::roomsGetFromGeneration());

		std::cout << i << "\n";
		if (generationSucceeded) {
			break;
		}
	}

	StructureGrid structureGrid = StructureGrid(structureSize.x, structureSize.y, structureType->structureType, structurePosition, structureRotation);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {

			StructureCell cell = StructureCell("Null", 0.f);

			if (wallGrid[x][y]) {
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

				cell.type = WallGenerator::cellTypeGetFromWallType(wallData.first);
				cell.rotation = wallData.second;
			}
			else if (roomTypeGrid.cellGet(x, y).type != RoomType::Null) {
				switch (roomTypeGrid.cellGet(x, y).type) {
				case RoomType::Hallway:
					cell.type = "Hallway Marker";
					break;
				case RoomType::Bathroom:
					cell.type = "Bathroom Marker";
					break;
				case RoomType::Bedroom:
					cell.type = "Bedroom Marker";
					break;

				}
			}

			structureGrid.cellSet(x, y, cell);
		}
	}

	return structureGrid;
}

