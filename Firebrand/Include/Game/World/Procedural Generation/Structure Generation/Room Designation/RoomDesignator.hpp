#ifndef __ROOM_DESIGNATOR_H__
#define __ROOM_DESIGNATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include "RoomType.hpp"

namespace RoomDesignator {

	void roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType);

	RoomTypeGrid structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRectVector roomRectsVector);
};

#endif