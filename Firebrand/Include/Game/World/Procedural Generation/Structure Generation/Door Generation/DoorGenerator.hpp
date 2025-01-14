#ifndef __DOOR_GENERATOR_H__
#define __DOOR_GENERATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include <set>

typedef std::vector<std::vector<bool>> DoorGrid2D;

namespace DoorGenerator {

	DoorGrid2D doorsGenerate(const WallGrid2D wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, RoomRectVector& roomRectsVector);
};

#endif