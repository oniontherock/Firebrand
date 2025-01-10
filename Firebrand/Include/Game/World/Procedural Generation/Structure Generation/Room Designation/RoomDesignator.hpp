#ifndef __ROOM_DESIGNATOR_H__
#define __ROOM_DESIGNATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include <set>

namespace RoomDesignator {
	
	RoomType pointTypeGet(RoomTypeGrid& roomTypeGrid, const sf::Vector2u point);
	RoomType roomTypeGet(RoomTypeGrid& roomTypeGrid, const RoomRect roomRect);

	void areaFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const sf::Vector2u pointStart, const RoomType roomType);
	void roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType);

	std::set<RoomType> roomNeighborTypesGet(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect);

	RoomTypeGrid structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector);
};

#endif