#include "WallGenerator.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <cmath>
#include <functional>
#include <iostream>

std::pair<WallGenerator::WallType, float> WallGenerator::wallDataGetFromSurroundings(SurroundingWallsVector wallStates) {
	WallType wallType;
	float wallRotation = 0.f;

	// check all
	if (wallStates[1] && wallStates[3] && wallStates[5] && wallStates[7]) {
		wallType = JunctionPlus;
	}
	/// Check T shapes
	// check right facing T
	else if (wallStates[1] && wallStates[5] && wallStates[7]) {
		wallType = JunctionT;
		wallRotation = 0;
	}
	// check up facing T
	else if (wallStates[1] && wallStates[3] && wallStates[5]) {
		wallType = JunctionT;
		wallRotation = -Mathf::PI / 2.f;
	}
	// check left facing T
	else if (wallStates[1] && wallStates[3] && wallStates[7]) {
		wallType = JunctionT;
		wallRotation = Mathf::PI;
	}
	// check down facing T
	else if (wallStates[3] && wallStates[5] && wallStates[7]) {
		wallType = JunctionT;
		wallRotation = Mathf::PI / 2.f;
	}
	/// End of check T shapes
	/// Check straights
	// horizontal
	else if (wallStates[3] && wallStates[5]) {
		wallType = Straight;
	}
	// vertical
	else if (wallStates[1] && wallStates[7]) {
		wallType = Straight;
		wallRotation = Mathf::PI / 2.f;
	}
	/// End of check straights
	/// Check corners
	// bottom and right
	else if (wallStates[5] && wallStates[7]) {
		wallType = Corner;
		wallRotation = 0;
	}
	// bottom and left
	else if (wallStates[3] && wallStates[7]) {
		wallType = Corner;
		wallRotation = Mathf::PI / 2.f;
	}
	// top and left
	else if (wallStates[1] && wallStates[3]) {
		wallType = Corner;
		wallRotation = Mathf::PI;
	}
	// top and right
	else if (wallStates[1] && wallStates[5]) {
		wallType = Corner;
		wallRotation = -Mathf::PI / 2.f;
	}
	/// End of check corners
	/// Check singles
	// right
	else if (wallStates[5]) {
		wallType = Single;
		wallRotation = 0;
	}
	// top
	else if (wallStates[1]) {
		wallType = Single;
		wallRotation = -Mathf::PI / 2.f;
	}
	// left
	else if (wallStates[3]) {
		wallType = Single;
		wallRotation = Mathf::PI;
	}
	// down
	else if (wallStates[7]) {
		wallType = Single;
		wallRotation = Mathf::PI / 2.f;
	}
	/// End of check singles
	else {
		wallType = None;
	}

	return std::pair(wallType, wallRotation);
}
StructureCellType WallGenerator::cellTypeGetFromWallType(WallType wallType) {
	switch (wallType) {
	case None:
		return "Null";
	case Single:
		return "Wall Single";
	case Straight:
		return "Wall Straight";
	case Corner:
		return "Wall Corner";
	case JunctionT:
		return "Wall Junction T";
	case JunctionPlus:
		return "Wall Junction Plus";
	default:
		return "Null";
	}
}

WallGenerator::RoomSizePair WallGenerator::roomMinMaxSizeGetFromType(StructureType structureType) {
	switch (structureType) {
	case StructureType::Home:
		return RoomSizePair(sf::Vector2u(6, 6), sf::Vector2u(8, 8));
	case StructureType::Barn:
		return RoomSizePair(sf::Vector2u(UINT16_MAX, 6), sf::Vector2u(UINT16_MAX, 8));
	case StructureType::Shed:
		return RoomSizePair(sf::Vector2u(0, 0), sf::Vector2u(0, 0));
	default:
		return RoomSizePair(sf::Vector2u(3, 3), sf::Vector2u(8, 8));
	}
}
WallGenerator::RoomCountPair WallGenerator::roomMinMaxCountGetFromType(StructureType structureType) {
	switch (structureType) {
	case StructureType::Home:
		return RoomCountPair(16, 24);
	case StructureType::Barn:
		return RoomCountPair(0, 2);
	case StructureType::Shed:
		return RoomCountPair(0, 0);
	default:
		return RoomCountPair(2, 6);
	}
}
WallGenerator::RoomContactCountPair WallGenerator::roomMinMaxContactCountGetFromType(StructureType structureType) {
	switch (structureType) {
	case StructureType::Home:
		return RoomContactCountPair(2, 4);
	case StructureType::Barn:
		return RoomContactCountPair(3, 3);
	case StructureType::Shed:
		return RoomContactCountPair(0, 0);
	default:
		return RoomContactCountPair(1, 4);
	}
}

sf::Vector2u WallGenerator::roomSizeGetFromType(StructureType structureType) {
	RoomSizePair roomSizeMinMax = roomMinMaxSizeGetFromType(structureType);

	// get room dimensions in the roomSizeMinMax range, we do plus one because getRange returns a value in the range of A to B-1.
	uint16_t sizeX = RNGu16::getRange(roomSizeMinMax.first.x, roomSizeMinMax.second.x + 1);
	uint16_t sizeY = RNGu16::getRange(roomSizeMinMax.first.y, roomSizeMinMax.second.y + 1);

	return sf::Vector2u(sizeX, sizeY);
}
uint16_t WallGenerator::roomCountGetFromType(StructureType structureType) {
	RoomCountPair roomCountMinMax = roomMinMaxCountGetFromType(structureType);

	// get room count in range, we do plus one because getRange returns a value in the range of A to B-1.
	return RNGu16::getRange(roomCountMinMax.first, roomCountMinMax.second + 1);
}
uint16_t WallGenerator::roomContactCountGetFromType(StructureType structureType) {
	RoomContactCountPair roomContactCountMinMax = roomMinMaxContactCountGetFromType(structureType);

	// get room contact count in range, we do plus one because getRange returns a value in the range of A to B-1.
	return RNGu16::getRange(roomContactCountMinMax.first, roomContactCountMinMax.second + 1);
}

void WallGenerator::roomWallsApplyToWallGrid(WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect, const bool solid) {
	for (uint32_t x = 0; x < roomRect.width; x++) {
		for (uint32_t y = 0; y < roomRect.height; y++) {

			sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + x, roomRect.top + y);

			if (((x == 0 || x == roomRect.width - 1) || (y == 0 || y == roomRect.height - 1)) || solid) {
				wallGrid[cellPosition.x][cellPosition.y] = true;
			}
		}
	}
}

std::vector<bool> WallGenerator::roomRectGetDoubleWalls(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect) {
	
	// vector of 4 booleans for which faces have wall doubling, order is: right, top, left, bottom.
	std::vector<bool> doubleFaces(4, false);

	// iterate right face
	for (uint16_t offsetY = 1; offsetY < roomRect.height - 1; offsetY++) {
		sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + (roomRect.width - 1) + 1, roomRect.top + offsetY);

		if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;

		bool isWall = wallGrid[cellPosition.x][cellPosition.y];

		if (isWall) {
			doubleFaces[0] = true;
			break;
		}
	}
	// iterate top face
	for (uint16_t offsetX = 1; offsetX < roomRect.width - 1; offsetX++) {
		sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + offsetX, roomRect.top - 1);

		if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;

		bool isWall = wallGrid[cellPosition.x][cellPosition.y];

		if (isWall) {
			doubleFaces[1] = true;
			break;
		}
	}
	// iterate left face
	for (uint16_t offsetY = 1; offsetY < roomRect.height - 1; offsetY++) {
		sf::Vector2i cellPosition = sf::Vector2i(roomRect.left - 1, roomRect.top + offsetY);

		if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;

		bool isWall = wallGrid[cellPosition.x][cellPosition.y];

		if (isWall) {
			doubleFaces[2] = true;
			break;
		}
	}
	// iterate bottom face
	for (uint16_t offsetX = 1; offsetX < roomRect.width - 1; offsetX++) {
		sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + offsetX, roomRect.top + (roomRect.height - 1) + 1);

		if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;

		bool isWall = wallGrid[cellPosition.x][cellPosition.y];

		if (isWall) {
			doubleFaces[3] = true;
			break;
		}
	}

	return doubleFaces;
}
bool WallGenerator::roomRectHasDoubleWalls(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect) {
	std::vector<bool> doubleFaces = roomRectGetDoubleWalls(wallGrid, structureSize, roomRect);

	for (uint16_t i = 0; i < doubleFaces.size(); i++) {
		if (doubleFaces[i]) return true;
	}
	return false;
}

RoomRect WallGenerator::roomRectFixDoubleWalls(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect, const sf::Vector2u roomDesiredSize, const uint16_t resizeTolerance) {

	std::vector<bool> doubleFaces = roomRectGetDoubleWalls(wallGrid, structureSize, roomRect);

	RoomRect roomRectAdjusted = roomRect;

	// the probability of whether a roomRect will move resize towards a double wall, thus merging, or resize away, dodging, a higher value means more merges
	float mergeVsDodgeProbability = 0.5f;

	// check right
	if (doubleFaces[0]) {
		if (RNGf::probability(mergeVsDodgeProbability)) {
			roomRectAdjusted.width++;
		}
		else {
			roomRectAdjusted.width--;
		}
	}
	// check top
	if (doubleFaces[1]) {
		if (RNGf::probability(mergeVsDodgeProbability)) {
			roomRectAdjusted.top--;
			roomRectAdjusted.height++;
		}
		else {
			roomRectAdjusted.top++;
			roomRectAdjusted.height--;
		}
	}
	// check left
	if (doubleFaces[2]) {
		if (RNGf::probability(mergeVsDodgeProbability)) {
			roomRectAdjusted.left--;
			roomRectAdjusted.width++;
		}
		else {
			roomRectAdjusted.left++;
			roomRectAdjusted.width--;
		}
	}
	// check bottom
	if (doubleFaces[3]) {
		if (RNGf::probability(mergeVsDodgeProbability)) {
			roomRectAdjusted.height++;
		}
		else {
			roomRectAdjusted.height--;
		}
	}

	// return error RoomRect if the width or height is too little
	if (roomRectAdjusted.width <= 2 || roomRectAdjusted.height <= 2) return RoomRect(0, 0, 0, 0);
	// return error RoomRect if the roomRect's size has differed more than resizeTolerance allows
	if (abs(int32_t(roomDesiredSize.x) - roomRectAdjusted.width) + abs(int32_t(roomDesiredSize.y) - roomRectAdjusted.height) > resizeTolerance) return RoomRect(0, 0, 0, 0);
	// return error RoomRect if roomRect exceeds level bounds
	if (
		(roomRectAdjusted.left < 0 || roomRectAdjusted.left + (roomRectAdjusted.width - 1) >= structureSize.x) ||
		(roomRectAdjusted.top < 0 || roomRectAdjusted.top + (roomRectAdjusted.height - 1) >= structureSize.y)) return RoomRect(0, 0, 0, 0);

	return roomRectAdjusted;
}
RoomRectVector WallGenerator::roomRectVectorLast{};

RoomRectVector WallGenerator::roomsGetFromGeneration() {
	return roomRectVectorLast;
}

sf::IntRect WallGenerator::roomGenerate(WallGrid2D& wallGrid, sf::Vector2u structureSize, sf::Vector2u roomSize, uint16_t roomContactCount, bool fullContact) {

	// clamp room width to structure width
	if (roomSize.x >= structureSize.x) {
		roomSize.x = structureSize.x - 1;
	}
	// clamp room height to structure height
	if (roomSize.y >= structureSize.y) {
		roomSize.y = structureSize.y - 1;
	}

	// rectangle of the room inside the wallGrid
	RoomRect roomRect = RoomRect(0, 0, roomSize.x, roomSize.y);
	
	// the current amount of roomRect faces touching walls in the wallGrid
	uint16_t contactCount = 0;

	uint32_t breaker = 1000;
	do {
		// get position of room rect inside structure, always fits inside of structure
		roomRect.left = RNGu16::getRange(0, structureSize.x - (roomSize.x - 1));
		roomRect.top = RNGu16::getRange(0, structureSize.y - (roomSize.y - 1));

		// the way we calculate contact depends on the fullContact variable, basically, true means we calculate by subtraction, false means we calculate by addition.
		// so fullContact means we start assuming we are fully contacted on every side of the room, and we subtract by 1 every time we find a side which isn't contacted.
		// and no fullContact assumes we are touching no walls, and adds by 1 every time it finds a wall on a side of the room
		contactCount = fullContact ? 4 : 0;

		bool doubleWallFixFailed = false;
		for (uint16_t i = 0; i < 512; i++) {
			roomRect = roomRectFixDoubleWalls(wallGrid, structureSize, roomRect, roomSize, 2);

			if (roomRect == RoomRect(0, 0, 0, 0)) {
				doubleWallFixFailed = true;
				break;
			}
		}
		// skip if the double wall fix failed
		if (doubleWallFixFailed) continue;
		// double check to make sure the roomRect has no double walls, skip if it does
		if (roomRectHasDoubleWalls(wallGrid, structureSize, roomRect)) continue;

		// iterate right face
		for (uint16_t offsetY = 0; offsetY < roomRect.height; offsetY++) {
			sf::Vector2u cellPosition = sf::Vector2u(roomRect.left + (roomRect.width - 1), roomRect.top + offsetY);

			bool isWall = wallGrid[cellPosition.x][cellPosition.y];

			if ((!isWall && fullContact) || (isWall && !fullContact)) {
				contactCount += fullContact ? -1 : +1;
				break;
			}
		}
		// iterate top face
		for (uint16_t offsetX = 0; offsetX < roomRect.width; offsetX++) {
			sf::Vector2u cellPosition = sf::Vector2u(roomRect.left + offsetX, roomRect.top);

			bool isWall = wallGrid[cellPosition.x][cellPosition.y];

			if ((!isWall && fullContact) || (isWall && !fullContact)) {
				contactCount += fullContact ? -1 : +1;
				break;
			}
		}
		// iterate left face
		for (uint16_t offsetY = 0; offsetY < roomRect.height; offsetY++) {
			sf::Vector2u cellPosition = sf::Vector2u(roomRect.left, roomRect.top + offsetY);

			bool isWall = wallGrid[cellPosition.x][cellPosition.y];

			if ((!isWall && fullContact) || (isWall && !fullContact)) {
				contactCount += fullContact ? -1 : +1;
				break;
			}
		}
		// iterate bottom face
		for (uint16_t offsetX = 0; offsetX < roomRect.width; offsetX++) {
			sf::Vector2u cellPosition = sf::Vector2u(roomRect.left + offsetX, roomRect.top + (roomRect.height - 1));

			bool isWall = wallGrid[cellPosition.x][cellPosition.y];

			if ((!isWall && fullContact) || (isWall && !fullContact)) {
				contactCount += fullContact ? -1 : +1;
				break;
			}
		}

	} while ((contactCount != roomContactCount) && ((--breaker) > 0));


	// if we didn't find a valid location, return an error rect
	if (breaker <= 0) return RoomRect(0, 0, 0, 0);

	return roomRect;
}

RoomRectVector WallGenerator::roomsGenerate(WallGrid2D& wallGrid, StructureType structureType, sf::Vector2u structureSize) {
	uint16_t roomsCount = roomCountGetFromType(structureType);

	std::vector<RoomRect> roomRectsVector;
	for (uint16_t roomIndCur = 0; roomIndCur < roomsCount; roomIndCur++) {

		uint16_t breaker = 1000;

		sf::Vector2u roomSize;
		uint16_t roomContactCount;
		do {
			roomSize = roomSizeGetFromType(structureType);
			roomContactCount = roomContactCountGetFromType(structureType);

			RoomRect roomRect = roomGenerate(wallGrid, structureSize, roomSize, roomContactCount, true);

			if (roomRect == RoomRect(0, 0, 0, 0)) continue;

			bool roomRectPosIsValid = true;
			for (uint16_t i = 0; i < roomRectsVector.size(); i++) {
				if (RoomRect(roomRect.left + 1, roomRect.top + 1, roomRect.width - 2, roomRect.height - 2).intersects(roomRectsVector[i])) {
					roomRectPosIsValid = false;
					break;
				}
			}

			if (roomRectPosIsValid) {
				roomWallsApplyToWallGrid(wallGrid, structureSize, roomRect, false);
				roomRectsVector.push_back(roomRect);
				break;
			}
		} while ((--breaker) > 0);
	}

	return roomRectsVector;
}

WallGrid2D WallGenerator::wallsGenerate(StructureType structureType, sf::Vector2u structureSize) {

	// initialize wall grid
	WallGrid2D wallGrid = WallGrid2D(structureSize.x, WallGrid1D(structureSize.y, false));

	// solidify edges of wallGrid
	roomWallsApplyToWallGrid(wallGrid, structureSize, RoomRect(0, 0, structureSize.x, structureSize.y));

	roomRectVectorLast = roomsGenerate(wallGrid, structureType, structureSize);

	return wallGrid;
}

