#include "WallGenerator.hpp"
#include <Auxiliary/Math.hpp>


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
const char* WallGenerator::cellTypeGetFromWallType(WallType wallType) {
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




