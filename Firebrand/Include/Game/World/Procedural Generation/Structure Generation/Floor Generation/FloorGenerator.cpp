#include "FloorGenerator.hpp"
#include "Auxiliary/Math.hpp"

void FloorGenerator::floorMarkHorizontal(RoomTypeGrid& roomTypeGrid, FloorGrid& floorGrid, const sf::Vector2u structureSize, sf::Vector2u cell, std::string leftType, std::string rightType) {
	floorGrid.cellGet(cell.x, cell.y) = leftType;
	floorGrid.cellGet(cell.x, cell.y + 1) = leftType;

	floorGrid.cellGet(cell.x + 1, cell.y) = rightType;
	floorGrid.cellGet(cell.x + 1, cell.y + 1) = rightType;
}
void FloorGenerator::floorMarkVertical(RoomTypeGrid& roomTypeGrid, FloorGrid& floorGrid, const sf::Vector2u structureSize, sf::Vector2u cell, std::string upType, std::string downType) {
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
			sf::Vector2u structureCellPos = sf::Vector2u(trunc(x / 2u), trunc(y / 2u));

			RoomType roomType = roomTypeGrid.cellGet(structureCellPos).type;

			RoomTypeInstance roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(roomType);

			if (roomType != Null) {
				floorGrid.cellSet(x, y, roomTypeInstance.floorType);
			}
			else {

				RoomType neighborType = RoomType::Null;

				uint16_t i = 0;
				for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
					for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

						// skip if offset is not even (I.E. diagonal or 0,0)
						if (Mathi32::isEven(abs(offsetX + offsetY))) continue;
					
						i++;

						int16_t offsettedX = int16_t(x) + offsetX;
						int16_t offsettedY = int16_t(y) + offsetY;

						// skip if cell not in floor grid
						if (!floorGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

						// get neighbor's position as a structureCell
						sf::Vector2u neighborStructureCellPos = sf::Vector2u(trunc(offsettedX / 2u), trunc(offsettedY / 2u));
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
				}
			}



			//// get validity of all sides of the cell
			//bool rightIsValid = roomTypeGrid.cellPosIsInGrid(x + 1, y);
			//bool upIsValid = roomTypeGrid.cellPosIsInGrid(x, y - 1);
			//bool leftIsValid = roomTypeGrid.cellPosIsInGrid(x - 1, y);
			//bool downIsValid = roomTypeGrid.cellPosIsInGrid(x, y + 1);

			//// initialize types for all sides of the cell
			//RoomType rightType = RoomType::Null;
			//RoomType upType = RoomType::Null;
			//RoomType leftType = RoomType::Null;
			//RoomType downType = RoomType::Null;

			//// get the type at a given side of the cell if it is valid
			//if (rightIsValid) {
			//	rightType = roomTypeGrid.cellGet(x + 1, y).type;
			//}
			//if (upIsValid) {
			//	upType = roomTypeGrid.cellGet(x, y - 1).type;
			//}
			//if (leftIsValid) {
			//	leftType = roomTypeGrid.cellGet(x - 1, y).type;
			//}
			//if (downIsValid) {
			//	downType = roomTypeGrid.cellGet(x, y + 1).type;
			//}

			//// get floor types of every side
			//std::string rightFloorType = RoomTypeRegistry::roomTypeInstanceGet(rightType).floorType;
			//std::string upFloorType = RoomTypeRegistry::roomTypeInstanceGet(rightType).floorType;
			//std::string leftFloorType = RoomTypeRegistry::roomTypeInstanceGet(rightType).floorType;
			//std::string downFloorType = RoomTypeRegistry::roomTypeInstanceGet(rightType).floorType;


			//// if there are no types up and down, mark floors horizontally
			//if (upType == Null && downType == Null) {
			//	floorMarkHorizontal(roomTypeGrid, floorGrid, structureSize, sf::Vector2u(x * 2, y * 2), leftFloorType, rightFloorType);
			//}
			//// if there are no types left and right, mark floors vertically
			//else if (leftType == Null && rightType == Null) {
			//	floorMarkVertical(roomTypeGrid, floorGrid, structureSize, sf::Vector2u(x * 2, y * 2), upFloorType, downFloorType);
			//}
			//// if one (or more) of the the up or down types are null, and neither left/right types are null, mark floor horizontally
			//else if ((upType == Null || downType == Null) && (leftType != Null && rightType != Null)) {
			//	floorMarkHorizontal(roomTypeGrid, floorGrid, structureSize, sf::Vector2u(x * 2, y * 2), leftFloorType, rightFloorType);
			//}
			//// if one (or more) of the the left or right types are null, and neither up/down types are null, mark floor vertically
			//else if ((leftType == Null || rightType == Null) && (upType != Null && downType != Null)) {
			//	floorMarkVertical(roomTypeGrid, floorGrid, structureSize, sf::Vector2u(x * 2, y * 2), leftFloorType, rightFloorType);
			//}
			//// if the up type isn't the same as the down type, mark floors horizontally
			//else if (upType != downType && leftType == rightType) {
			//	floorMarkHorizontal(roomTypeGrid, floorGrid, structureSize, sf::Vector2u(x * 2, y * 2), leftFloorType, rightFloorType);
			//}
			//// if the left type isn't the same as the right type, mark floors vertically
			//else if (leftType != rightType && upType == downType) {
			//	floorMarkVertical(roomTypeGrid, floorGrid, structureSize, sf::Vector2u(x * 2, y * 2), leftFloorType, rightFloorType);
			//}
		}
	}

	return floorGrid;
}
