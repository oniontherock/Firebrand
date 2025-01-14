#ifndef __ROOM_DESIGNATOR_H__
#define __ROOM_DESIGNATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include <set>

struct Vector2uLessThan
{
    bool operator()(const sf::Vector2u& lhs, const sf::Vector2u& rhs) const
    {
        // compare x coordinates first
        if (lhs.x < rhs.x)
            return true;
        if (lhs.x > rhs.x)
            return false;

        // if x coordinates are equal, compare y coordinates
        return lhs.y < rhs.y;
    }
};

namespace RoomDesignator {
	
	RoomType pointTypeGet(RoomTypeGrid& roomTypeGrid, const sf::Vector2u point);
	RoomType roomTypeGet(RoomTypeGrid& roomTypeGrid, const RoomRect roomRect);

	void areaFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const sf::Vector2u pointStart, const RoomType roomType);
	void roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType);

	std::set<RoomType> roomNeighborTypesGet(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect);

	// designates the types of rooms in a structure, returns a bool for whether the generation succeeded or not.
	bool structureRoomTypesDesignate(const WallGrid2D& wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector);
};

#endif