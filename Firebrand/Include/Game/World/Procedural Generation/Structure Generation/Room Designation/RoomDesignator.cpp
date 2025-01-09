#include "RoomDesignator.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <queue>
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

void RoomDesignator::areaFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const sf::Vector2u pointStart, const RoomType roomType) {

    std::queue<sf::Vector2u> pointsToCheckQueue;
    // set used to make sure we don't have duplicates in pointsToCheckQueue
    std::set<sf::Vector2u, Vector2uLessThan> pointsToCheckIds;

    pointsToCheckQueue.push(pointStart);
    pointsToCheckIds.insert(pointStart);

    sf::Vector2u pointCur;

    while (!pointsToCheckQueue.empty()) {

        pointCur = pointsToCheckQueue.front();
        pointsToCheckQueue.pop();

        roomTypeGrid.cellGet(pointCur).type = roomType;

        for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
            for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

                // skip if element is even, an even offset means it's either diagonal or (0, 0), which we don't want
                if (Mathi16::isEven(abs(offsetX + offsetY))) continue;

                int16_t offsettedX = pointCur.x + offsetX;
                int16_t offsettedY = pointCur.y + offsetY;

                // skip if offsetted point is out of bounds
                if ((offsettedX < 0 || offsettedX >= structureSize.x) || (offsettedY < 0 || offsettedY >= structureSize.y)) continue;
                // skip if cell is a wall
                if (wallGrid[offsettedX][offsettedY]) continue;

                sf::Vector2u offsettedPoint = sf::Vector2u(uint16_t(offsettedX), uint16_t(offsettedY));

                // if the point isn't queued for checking / already checked, queue it for checking
                if (!pointsToCheckIds.contains(offsettedPoint)) {
                    pointsToCheckQueue.push(offsettedPoint);
                    pointsToCheckIds.insert(offsettedPoint);
                }
            }
        }
    }
}

void RoomDesignator::roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType) {
    areaFillWithType(wallGrid, structureSize, roomTypeGrid, sf::Vector2u(roomRect.getPosition() + (roomRect.getSize() / 2)), roomType);
}

std::set<RoomType> RoomDesignator::roomNeighborTypesGet(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect) {
    std::set<RoomType> neighborTypes;

    // roomRect with each face expanded by 1
    RoomRect roomRectExpanded = RoomRect(roomRect.left - 1, roomRect.top - 1, roomRect.width + 2, roomRect.height + 2);

    for (uint16_t x = 0; x < roomRectExpanded.width; x++) {
        for (uint16_t y = 0; y < roomRectExpanded.height; y++) {
            if ((x == 0 || x == roomRectExpanded.width - 1) || (y == 0 || y == roomRectExpanded.height - 1)) {
                RoomType neighborRoomType = roomTypeGrid.cellGet(roomRectExpanded.left + x, roomRectExpanded.top + y).type;

                if (neighborRoomType != RoomType::Null) {
                    neighborTypes.insert(neighborRoomType);
                }
            }
        }
    }

    return neighborTypes;
}

RoomTypeGrid RoomDesignator::structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector) {
    RoomTypeGrid roomTypeGrid = RoomTypeGrid(structureSize.x, structureSize.y);
    
    // fill rooms
    for (uint16_t i = 0; i < roomRectsVector.size(); i++) {
        roomFillWithType(wallGrid, structureSize, roomTypeGrid, roomRectsVector[i], RoomType::Misc);
    }
    for (uint16_t x = 0; x < structureSize.x; x++) {
        for (uint16_t y = 0; y < structureSize.y; y++) {
            // if cell is null and it's not a wall, it must be a hallway, so fill it with a hallway
            if ((roomTypeGrid.cellGet(x, y).type == RoomType::Null) && (!wallGrid[x][y])) {
                areaFillWithType(wallGrid, structureSize, roomTypeGrid, sf::Vector2u(x, y), RoomType::Hallway);
            }
        }
    }

    return roomTypeGrid;
}

