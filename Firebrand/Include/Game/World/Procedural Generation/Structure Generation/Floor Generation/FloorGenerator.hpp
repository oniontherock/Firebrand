#ifndef __FLOOR_GENERATOR_H__
#define __FLOOR_GENERATOR_H__


#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include "../Wall Sections/WallSectionGenerator.hpp"
#include <set>


namespace FloorGenerator {

	enum FloorType {
		Null,
		Solid,
		Long,
		Corner,
	};

	typedef std::vector<FloorType> FloorGrid1D;
	typedef std::vector<FloorGrid1D> FloorGrid2D;
	// gets floor data from surroundings,
	// so for example, it checks for corners or long floors, and returns a rotation and type according to that,
	std::pair<FloorType, float> floorDataGetFromSurroundings(SurroundingWallsVector wallStates);
	StructureCellType cellTypeGetFromFloorType(FloorType wallType);

	FloorGrid2D floorsGenerate(RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize);
};





#endif