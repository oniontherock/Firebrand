#include "FloorGenerator.hpp"
#include "Auxiliary/Math.hpp"

void FloorGenerator::floorMarkHorizontal(FloorGrid& floorGrid, sf::Vector2u cell, std::string leftType, std::string rightType) {
	floorGrid.cellGet(cell.x, cell.y) = leftType;
	floorGrid.cellGet(cell.x, cell.y + 1) = leftType;

	floorGrid.cellGet(cell.x + 1, cell.y) = rightType;
	floorGrid.cellGet(cell.x + 1, cell.y + 1) = rightType;
}
void FloorGenerator::floorMarkVertical(FloorGrid& floorGrid, sf::Vector2u cell, std::string upType, std::string downType) {
	floorGrid.cellGet(cell.x, cell.y) = upType;
	floorGrid.cellGet(cell.x + 1, cell.y) = upType;

	floorGrid.cellGet(cell.x, cell.y + 1) = downType;
	floorGrid.cellGet(cell.x + 1, cell.y + 1) = downType;
}

FloorGrid FloorGenerator::floorsGenerate(RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize) {

	using enum RoomType;


	const sf::Vector2u floorGridSize = structureSize * 2u;
	FloorGrid floorGrid = FloorGrid(floorGridSize.x, floorGridSize.y);
	const sf::Vector2f floorGridCellSize = floorGrid.cellsGetSize();


	for (uint16_t x = 0; x < floorGridSize.x; x++) {
		for (uint16_t y = 0; y < floorGridSize.y; y++) {

			// floor cell coordinates converted to coordinates as a structure cell
			sf::Vector2u structureCellPos = sf::Vector2u(uint32_t(trunc(x / 2u)), uint32_t(trunc(y / 2u)));

			RoomType roomType = roomTypeGrid.cellGet(structureCellPos).type;

			RoomTypeInstance roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(roomType);

			if (roomType != Null) {
				floorGrid.cellSet(x, y, roomTypeInstance.floorType);
				continue;
			}

			RoomType neighborType = RoomType::Null;

			for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
				for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

					// skip if offset is even (I.E. diagonal or 0,0)
					if (Mathi32::isEven(abs(offsetX + offsetY))) continue;
					

					int16_t offsettedX = int16_t(x) + offsetX;
					int16_t offsettedY = int16_t(y) + offsetY;

					// skip if cell not in floor grid
					if (!floorGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

					// get neighbor's position as a structureCell
					sf::Vector2u neighborStructureCellPos = sf::Vector2u(uint32_t(trunc(offsettedX / 2u)), uint32_t(trunc(offsettedY / 2u)));
					// if the neighbor's structure cell pos is the same as ours, skip
					if (structureCellPos == neighborStructureCellPos) continue;

					RoomType neighborTypeCur = roomTypeGrid.cellGet(neighborStructureCellPos).type;

					// skip if neighbor type is null
					if (neighborTypeCur == Null) continue;

					neighborType = neighborTypeCur;
					break;
				}
			}

			if (neighborType != Null) {
				RoomTypeInstance neighborRoomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(neighborType);

				floorGrid.cellSet(x, y, neighborRoomTypeInstance.floorType);
				continue;
			}


			RoomType neighborDiagonalType = RoomType::Null;

			for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
				for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

					// skip if offset is odd (I.E. not diagonal), or skip  if offset is 0,0
					if (Mathi32::isOdd(abs(offsetX + offsetY)) || (offsetX == 0 && offsetY == 0)) continue;

					int16_t offsettedX = int16_t(x) + offsetX;
					int16_t offsettedY = int16_t(y) + offsetY;

					// skip if cell not in floor grid
					if (!floorGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

					// get neighbor's position as a structureCell
					sf::Vector2u neighborStructureCellPos = sf::Vector2u(uint32_t(trunc(offsettedX / 2u)), uint32_t(trunc(offsettedY / 2u)));
					// if the neighbor's structure cell pos is the same as ours, skip
					if (structureCellPos == neighborStructureCellPos) continue;

					RoomType neighborTypeCur = roomTypeGrid.cellGet(neighborStructureCellPos).type;

					// skip if neighbor type is null
					if (neighborTypeCur == Null) continue;

					neighborDiagonalType = neighborTypeCur;
					break;
				}
			}

			if (neighborDiagonalType != Null) {
				RoomTypeInstance neighborRoomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(neighborDiagonalType);

				floorGrid.cellSet(x, y, neighborRoomTypeInstance.floorType);
				continue;
			}
		}
	}

	return floorGrid;
}
