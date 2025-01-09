#ifndef __ROOM_DESIGNATOR_H__
#define __ROOM_DESIGNATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include "RoomType.hpp"
#include "RoomType.hpp"

namespace RoomDesignator {

	void areaFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const sf::Vector2u pointStart, const RoomType roomType);
	void roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType);

	std::set<RoomType> roomNeighborTypesGet(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect);

	RoomTypeGrid structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector);
};

#endif