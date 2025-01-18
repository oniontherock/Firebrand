#include "Door Generation/DoorGenerator.hpp"
#include "Floor Generation/FloorGenerator.hpp"
#include "Room Designation/RoomDesignator.hpp"
#include "StructureGenerator.hpp"
#include "Wall Sections/WallSectionGenerator.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>


Structure StructureGenerator::structureGenerate(StructureTypeBase* structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2u structureSize) {

	WallGrid2D wallGrid;

	RoomTypeGrid roomTypeGrid = RoomTypeGrid(structureSize.x, structureSize.y);

	RoomRectVector roomRectsVector;
	for (uint16_t i = 0; i < 64; i ++) {

		wallGrid = WallGenerator::wallsGenerate(structureType, structureSize);

		// get roomRects from wall generation
		roomRectsVector = WallGenerator::roomsGetFromGeneration();
		// shuffle roomRectsVector
		for (int16_t j = int16_t(roomRectsVector.size()) - 1; j >= 0; j--) {

			// generate the random index 
			uint16_t k = RNGu16::getUnder(j + 1);

			RoomRect temp = roomRectsVector[j];
			roomRectsVector[j] = roomRectsVector[k];
			roomRectsVector[k] = temp;
		}

		bool generationSucceeded = RoomDesignator::structureRoomTypesDesignate(wallGrid, roomTypeGrid, structureSize, roomRectsVector);


		if (generationSucceeded) {
			break;
		}
	}
	
	WallSectionGenerator::WallSectionGrid2D wallSectionGrid = WallSectionGenerator::wallSectionsGet(wallGrid, roomTypeGrid, structureSize);
	DoorGenerator::DoorGrid2D doorGrid = DoorGenerator::doorsGenerate(wallSectionGrid, roomTypeGrid, structureSize, roomRectsVector);

	StructureGrid structureGrid = StructureGrid(structureSize.x, structureSize.y);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {

			const RoomType roomType = roomTypeGrid.cellGet(x, y).type;

			StructureCell cell = StructureCell("Null", 0.f);

#pragma region Get Wall States
			std::vector<bool> wallStates(9, false);

			for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
				for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

					// if the offset is even it means it's either a diagonal, or (0, 0), which we don't want, so skip if it is
					if (Mathi32::isEven(abs(offsetX + offsetY))) continue;

					int16_t offsettedX = x + offsetX;
					int16_t offsettedY = y + offsetY;

					if (!structureGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

					uint16_t flattenedIndex = uint16_t(((offsetY + 1) * 3) + (offsetX + 1));

					wallStates[flattenedIndex] = wallGrid[offsettedX][offsettedY];
				}
			}
#pragma endregion

			if (doorGrid[x][y]) {
				std::pair<WallGenerator::WallType, float> wallData = WallGenerator::wallDataGetFromSurroundings(wallStates);

				cell.type = "Door Wooden";
				cell.rotation = wallData.second;
			}
			else if (wallGrid[x][y]) {


				// get wall data
				std::pair<WallGenerator::WallType, float> wallData = WallGenerator::wallDataGetFromSurroundings(wallStates);

				cell.type = WallGenerator::cellTypeGetFromWallType(wallData.first);
				cell.rotation = wallData.second;

				//if (wallSectionGrid[x][y].first != RoomType::Null && wallSectionGrid[x][y].second != RoomType::Null) {	

				//		cell.type = "Misc Marker";
				//}
			}
			else if (roomType != RoomType::Null) {

				//const RoomTypeInstance& roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(roomType);

				//std::pair<:FloorType, float> floorData = FloorGenerator::floorDataGetFromSurroundings(wallStates);

				//cell.type = FloorGenerator::cellTypeGetFromFloorType(floorData.first);
				//cell.rotation = floorData.second;

				//std::cout << "cell rotation: " << cell.rotation << "\n";

				//cell.type += std::string(" ") + roomTypeInstance.floorType;

				//switch (roomTypeGrid.cellGet(x, y).type) {
				//case RoomType::Hallway:
				//	cell.type = "Hallway Marker";
				//	break;
				//case RoomType::Bathroom:
				//	cell.type = "Bathroom Marker";
				//	break;
				//case RoomType::Bedroom:
				//	cell.type = "Bedroom Marker";
				//	break;
				//case RoomType::LivingRoom:
				//	cell.type = "LivingRoom Marker";
				//	break;
				////case RoomType::Laboratory:
				////	cell.type = "Laboratory Marker";
				////	break;
				//}
			}

			structureGrid.cellSet(x, y, cell);
		}
	}

	return Structure(structureGrid, FloorGenerator::floorsGenerate(roomTypeGrid, structureSize), structureType->structureType, structurePosition, structureRotation);
}

