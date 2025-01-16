#include "FloorGenerator.hpp"
#include "Auxiliary/Math.hpp"


std::pair<FloorGenerator::FloorType, float> FloorGenerator::floorDataGetFromSurroundings(SurroundingWallsVector wallStates) {
	
	// inverted version of wallStates, we use this instead of wall states because we are mostly checking for empty space, instead of walls
	std::vector<bool> wallStatesInverted = wallStates;

	for (uint16_t i = 0; i < wallStatesInverted.size(); i++) {
		wallStatesInverted[i] = !wallStates[i];
	}

	FloorType floorType;
	float floorRotation = 0.f;
	 

	/// Check corner shapes
	// check bottom right corner
	if (wallStatesInverted[6] && wallStatesInverted[8] && wallStatesInverted[9]) {
		floorType = Corner;
		floorRotation = 0;
	}
	// check top right corner
	else if (wallStatesInverted[2] && wallStatesInverted[3] && wallStatesInverted[6]) {
		floorType = Corner;
		floorRotation = -Mathf::PI / 2.f;
	}
	// check top left corner
	else if (wallStatesInverted[1] && wallStatesInverted[2] && wallStatesInverted[4]) {
		floorType = Corner;
		floorRotation = Mathf::PI;
	}
	// check bottom left corner
	else if (wallStatesInverted[4] && wallStatesInverted[7] && wallStatesInverted[8]) {
		floorType = Corner;
		floorRotation = Mathf::PI / 2.f;
	}
	/// End of check corner shapes
	/// Check longs
	// right
	else if (wallStatesInverted[6]) {
		floorType = Long;
		floorRotation = 0;
	}
	// top
	else if (wallStatesInverted[2]) {
		floorType = Long;
		floorRotation = -Mathf::PI / 2.f;
	}
	// left
	else if (wallStatesInverted[4]) {
		floorType = Long;
		floorRotation = Mathf::PI;
	}
	// bottom
	else if (wallStatesInverted[8]) {
		floorType = Long;
		floorRotation = Mathf::PI / 2.f;
	}
	/// End of check corners
	else {
		floorType = Solid;
	}

	return std::pair(floorType, floorRotation);
}
StructureCellType FloorGenerator::cellTypeGetFromFloorType(FloorType wallType) {
	// note that, although we could use a vector that we access by WallType, we use a switch statement because reorder the WallType enum wouldn't mess up anything
	switch (wallType) {
	case Solid:
		return "Floor Solid";
	case Long:
		return "Floor Long";
	case Corner:
		return "Floor Corner";
	default:
		return "Null";
	}
}

FloorGenerator::FloorGrid2D FloorGenerator::floorsGenerate(RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize) {

	FloorGrid2D floorGrid = FloorGrid2D(structureSize.x, FloorGrid1D(structureSize.y, Null));

	//for (uint16_t x = 0; x < structureSize.x; x++) {
	//	for (uint16_t y = 0; y < structureSize.y; y++) {
	//		floorGrid[x][y] = WallGenerator:
	//	}
	//}

	return FloorGrid2D();
}