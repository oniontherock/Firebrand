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

RoomDesignator::RoomTypeVector RoomDesignator::roomTypeConnectionsGetFromRoomType(const RoomType roomType) {
    RoomTypeVector roomTypeVector;

    switch (roomType) {
    case RoomType::Null:
        break;
    case RoomType::Hallway:
        // hallways can connect to anything, so we just iterate over all the RoomTypes and add them all
        for (uint16_t i = 0; i < uint16_t(RoomType::RoomTypeSize); i++) {
            roomTypeVector.push_back(RoomType(i));
        }
        break;
    case RoomType::Bedroom:
        roomTypeVector.push_back(RoomType::Hallway);
        break;
    case RoomType::Bathroom:
        roomTypeVector.push_back(RoomType::Bathroom);
        roomTypeVector.push_back(RoomType::Hallway);
        roomTypeVector.push_back(RoomType::LivingRoom);
        roomTypeVector.push_back(RoomType::Laboratory);
        roomTypeVector.push_back(RoomType::Misc);
        break;
    case RoomType::LivingRoom:
        roomTypeVector.push_back(RoomType::Hallway);
        break;
    case RoomType::Laboratory:
        roomTypeVector.push_back(RoomType::Hallway);
        roomTypeVector.push_back(RoomType::Misc);
        break;
    default:
        break;
    }

    return roomTypeVector;
}
RoomDesignator::RoomTypeConnectionCountPair RoomDesignator::roomTypeConnectionCountMinMaxGetFromRoomType(const RoomType roomType) {
    switch (roomType) {/ 
    case RoomType::Null:
        return RoomTypeConnectionCountPair(0, 1);
    case RoomType::Hallway:
        return RoomTypeConnectionCountPair(1, 8);
    case RoomType::Bedroom:
        return RoomTypeConnectionCountPair(1, 3);
    case RoomType::Bathroom:
        return RoomTypeConnectionCountPair(1, 2);
    case RoomType::LivingRoom:
        return RoomTypeConnectionCountPair(2, 3);
    case RoomType::Laboratory:
        return RoomTypeConnectionCountPair(2, 4);
    case RoomType::Misc:
        return RoomTypeConnectionCountPair(1, 8);
    default:
        return RoomTypeConnectionCountPair(1, 8);
    }
}

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

RoomTypeGrid RoomDesignator::structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector) {
    RoomTypeGrid roomTypeGrid = RoomTypeGrid(structureSize.x, structureSize.y);
    
    // fill rooms
    for (uint16_t i = 0; i < roomRectsVector.size(); i++) {
        roomFillWithType(wallGrid, structureSize, roomTypeGrid, roomRectsVector[i], RNGf::probability(0.5f) ? RoomType::Bathroom : RoomType::Bedroom);
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

