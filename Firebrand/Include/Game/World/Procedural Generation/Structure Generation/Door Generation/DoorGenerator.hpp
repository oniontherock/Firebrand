#ifndef __DOOR_GENERATOR_H__
#define __DOOR_GENERATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include "../Wall Sections/WallSectionGenerator.hpp"
#include <set>


namespace DoorGenerator {
	typedef std::vector<bool> DoorGrid1D;
	typedef std::vector<DoorGrid1D> DoorGrid2D;

	void exteriorDoorsGenerate(RoomTypeGrid& roomTypeGrid, DoorGrid2D& doorGrid, const sf::Vector2u structureSize, const uint16_t desiredDoorCount);
	void roomDoorsGenerate(const WallSectionGenerator::WallSectionGrid2D& wallSectionGrid, RoomTypeGrid& roomTypeGrid, DoorGrid2D& doorGrid, const sf::Vector2u structureSize, RoomRectVector& roomRectsVector);

	DoorGrid2D doorsGenerate(const WallSectionGenerator::WallSectionGrid2D& wallSectionGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, RoomRectVector& roomRectsVector);
};

#endif