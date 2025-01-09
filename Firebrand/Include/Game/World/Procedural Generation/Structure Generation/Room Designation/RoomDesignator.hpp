#ifndef __ROOM_DESIGNATOR_H__
#define __ROOM_DESIGNATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include "RoomType.hpp"

namespace RoomDesignator {

	typedef std::vector<RoomType> RoomTypeVector;
	typedef std::pair<uint16_t, uint16_t> RoomTypeConnectionCountPair;

	// returns a RoomTypeVector that contains all the types of rooms that a room can connect to,
	// for example, a bathroom returns a bedroom and a hallway, and some others as well,
	RoomTypeVector roomTypeConnectionsGetFromRoomType(const RoomType roomType);
	// returns the minimum and maximum amount of connections a certain RoomType can have
	RoomTypeConnectionCountPair roomTypeConnectionCountMinMaxGetFromRoomType(const RoomType roomType);

	void areaFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const sf::Vector2u pointStart, const RoomType roomType);
	void roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType);

	RoomTypeGrid structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector);
};

#endif