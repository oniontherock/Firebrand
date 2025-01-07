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

void RoomDesignator::roomFillWithType(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType) {
	
	sf::Vector2u pointStart = sf::Vector2u(roomRect.getPosition() + (roomRect.getSize() / 2));

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

                // if the point isn't queued for checking / already checked, queue it for checking
                if (!pointsToCheckIds.contains(pointCur)) {
                    pointsToCheckQueue.push(pointCur);
                    pointsToCheckIds.insert(pointCur);
                }
            }
        }
    }
}

RoomTypeGrid RoomDesignator::structureRoomTypesDesignate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRectVector roomRectsVector) {
    for (uint16_t i = 0; i < roomRectsVector.size(); i++) {
        roomFillWithType(wallGrid, structureSize, roomTypeGrid, roomRectsVector[i], RNGf::probability(0.5f) ? RoomType::Bathroom : RoomType::Hallway);
    }
}

