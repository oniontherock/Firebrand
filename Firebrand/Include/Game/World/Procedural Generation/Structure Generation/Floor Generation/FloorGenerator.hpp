#ifndef __FLOOR_GENERATOR_H__
#define __FLOOR_GENERATOR_H__


#include "../Grid/Floor Grid/FloorGrid.hpp"
#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include "../Wall Sections/WallSectionGenerator.hpp"
#include <set>


struct FloorGenerator {
	static FloorGrid floorsGenerate(RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize);

private:
	static void floorMarkHorizontal(FloorGrid& floorGrid, sf::Vector2u cell, std::string leftType, std::string rightType);
	static void floorMarkVertical(FloorGrid& floorGrid, sf::Vector2u cell, std::string upType, std::string downType);
};





#endif