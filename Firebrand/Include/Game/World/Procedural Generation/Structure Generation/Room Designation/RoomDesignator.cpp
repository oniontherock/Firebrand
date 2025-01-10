#include "../Path Verification/PathVerifier.hpp"
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

RoomType RoomDesignator::pointTypeGet(RoomTypeGrid& roomTypeGrid, const sf::Vector2u point) {
    return roomTypeGrid.cellGet(point).type;
}
RoomType RoomDesignator::roomTypeGet(RoomTypeGrid& roomTypeGrid, const RoomRect roomRect) {
    return pointTypeGet(roomTypeGrid, sf::Vector2u(roomRect.left + (roomRect.width / 2u), roomRect.top + (roomRect.height / 2u)));
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

std::set<RoomType> RoomDesignator::roomNeighborTypesGet(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect) {
    std::set<RoomType> neighborTypes;

    // iterate right face
    for (uint16_t offsetY = 1; offsetY < roomRect.height - 1; offsetY++) {
        sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + (roomRect.width - 1) + 1, roomRect.top + offsetY);

        if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;
    
        neighborTypes.insert(roomTypeGrid.cellGet(cellPosition.x, cellPosition.y).type);
    }
    // iterate top face
    for (uint16_t offsetX = 1; offsetX < roomRect.width - 1; offsetX++) {
        sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + offsetX, roomRect.top - 1);

        if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;
       
        neighborTypes.insert(roomTypeGrid.cellGet(cellPosition.x, cellPosition.y).type);
    }
    // iterate left face
    for (uint16_t offsetY = 1; offsetY < roomRect.height - 1; offsetY++) {
        sf::Vector2i cellPosition = sf::Vector2i(roomRect.left - 1, roomRect.top + offsetY);

        if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;

        neighborTypes.insert(roomTypeGrid.cellGet(cellPosition.x, cellPosition.y).type);
    }
    // iterate bottom face
    for (uint16_t offsetX = 1; offsetX < roomRect.width - 1; offsetX++) {
        sf::Vector2i cellPosition = sf::Vector2i(roomRect.left + offsetX, roomRect.top + (roomRect.height - 1) + 1);

        if ((cellPosition.x < 0 || cellPosition.x >= structureSize.x) || (cellPosition.y < 0 || cellPosition.y >= structureSize.y)) continue;
        
        neighborTypes.insert(roomTypeGrid.cellGet(cellPosition.x, cellPosition.y).type);
    }

    return neighborTypes;
}

bool RoomDesignator::structureRoomTypesDesignate(const WallGrid2D& wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector) {
    roomTypeGrid = RoomTypeGrid(structureSize.x, structureSize.y);
    
    // fill rooms with misc
    for (uint16_t i = 0; i < roomRectsVector.size(); i++) {
        roomFillWithType(wallGrid, structureSize, roomTypeGrid, roomRectsVector[i], RoomType::Misc);
    }
    // fill hallways
    for (uint16_t x = 0; x < structureSize.x; x++) {
        for (uint16_t y = 0; y < structureSize.y; y++) {
            // if cell is null and it's not a wall, it must be a hallway, so fill it with a hallway
            if ((roomTypeGrid.cellGet(x, y).type == RoomType::Null) && (!wallGrid[x][y])) {
                areaFillWithType(wallGrid, structureSize, roomTypeGrid, sf::Vector2u(x, y), RoomType::Hallway);
            }
        }
    }
    
    // ensure all hallways can reach each other
    for (uint16_t aX = 0; aX < structureSize.x; aX++) {
        for (uint16_t aY = 0; aY < structureSize.y; aY++) {

            // skip if room not a hallway
            if (roomTypeGrid.cellGet(aX, aY).type != RoomType::Hallway) continue;

            for (uint16_t bX = 0; bX < structureSize.x; bX++) {
                for (uint16_t bY = 0; bY < structureSize.y; bY++) {

                    // skip if same cell
                    if ((aX == bX) && (aY == bY)) continue;

                    // skip if room not a hallway
                    if (roomTypeGrid.cellGet(bX, bY).type != RoomType::Hallway) continue;

                    // if hallway doesn't connect, return a failed designation
                    if (!PathVerifier::pointsConnect(wallGrid, structureSize, sf::Vector2u(aX, aY), sf::Vector2u(bX, bY))) {
                        return false;
                    }
                }
            }
        }
    }

    std::vector<uint16_t> addedTypes = std::vector<uint16_t>((uint16_t(RoomType::RoomTypeSize)));

    for (uint16_t i = 0; i < 8; i++) {

        // designate rooms
        for (uint16_t roomTypeCur = 3; roomTypeCur < uint16_t(RoomType::RoomTypeSize); roomTypeCur++) {
            const RoomTypeInstance& roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(RoomType(roomTypeCur));


            for (const auto& roomCur : roomRectsVector) {
                if (roomTypeGet(roomTypeGrid, roomCur) != RoomType::Misc) continue;

                std::set<RoomType> roomNeighbors = roomNeighborTypesGet(wallGrid, structureSize, roomTypeGrid, roomCur);

#pragma region Constraint Checking
                // bool for whether or not all of the roomType's constraints have been met
                bool roomTypeConstraintsMet = true;
                // check if the room has a RoomConnectionsAnd constraint
                if (roomTypeInstance.constraints.dataHas("RoomConnectionsAnd")) {

                    // get RoomConnectionsAnd constraint
                    std::set<RoomType> roomConnections = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsAnd");

                    // iterate over neighbors
                    for (const auto& roomNeighborCur : roomNeighbors) {
                        // erase the types of any neighbors from roomConnectionsAnd, effectively marking it as found
                        if (roomConnections.contains(roomNeighborCur)) {
                            roomConnections.erase(roomNeighborCur);
                        }
                    }
                    // if roomConnectionsAnd still has elements, I.E. not all RoomConnections were found, mark the roomTypesAndValidate variable as false
                    if (roomConnections.size() > 0) {
                        roomTypeConstraintsMet = false;
                    }
                }
                // check if the room has a RoomConnectionsOr constraint
                if (roomTypeInstance.constraints.dataHas("RoomConnectionsOr")) {

                    // get RoomConnectionsAnd constraint
                    std::set<RoomType> roomConnections = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsOr");

                    bool neighborFound = false;
                    // iterate over neighbors
                    for (const auto& roomNeighborCur : roomNeighbors) {
                        // check if a neighbor was found, and mark neighborFound as true if a neighbor was found
                        if (roomConnections.contains(roomNeighborCur)) {
                            neighborFound = true;
                            break;
                        }
                    }
                    // if no neighbors were found, mark the roomTypesAndValidate variable as false
                    if (!neighborFound) {
                        roomTypeConstraintsMet = false;
                    }
                }
                // check if the room has a RoomConnectionsNand constraint
                if (roomTypeInstance.constraints.dataHas("RoomConnectionsNand")) {

                    // get RoomConnectionsNand constraint
                    std::set<RoomType> roomConnections = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsNand");

                    // iterate over neighbors
                    for (const auto& roomNeighborCur : roomNeighbors) {
                        // if a neighbor that is not allowed in the Nand is found, mark roomTypeConstraintsMet as false
                        if (roomConnections.contains(roomNeighborCur)) {
                            roomTypeConstraintsMet = false;
                            break;
                        }
                    }
                }
                // check if the room has a RoomDimensionsRange constraint
                if (roomTypeInstance.constraints.dataHas("RoomDimensionsRange")) {

                    // get RoomDimensionsRange constraint
                    std::pair<RoomSize, RoomSize> roomDimensionsRange = roomTypeInstance.constraints.dataGet<std::pair<RoomSize, RoomSize>>("RoomDimensionsRange");

                    // ensure room's dimensions are within the type's allowed range
                    if ((roomCur.width < roomDimensionsRange.first.x || roomCur.width > roomDimensionsRange.second.x) ||
                        (roomCur.height < roomDimensionsRange.first.y || roomCur.height> roomDimensionsRange.second.y)) {
                        roomTypeConstraintsMet = false;
                    }
                }
                // check if the room has a RoomSizeRange constraint
                if (roomTypeInstance.constraints.dataHas("RoomSizeRange")) {

                    // get RoomSizeRange constraint
                    std::pair<uint16_t, uint16_t> roomSizeRange = roomTypeInstance.constraints.dataGet<std::pair<uint16_t, uint16_t>>("RoomSizeRange");

                    // get the amount of cells in the room
                    uint16_t roomSize = roomCur.width * roomCur.height;

                    // ensure room's size is within the type's allowed range
                    if (roomSize < roomSizeRange.first || roomSize > roomSizeRange.second) {
                        roomTypeConstraintsMet = false;
                    }
                }
                // check if room has a RoomTypeCount constraint
                if (roomTypeInstance.constraints.dataHas("RoomTypeCount")) {

                    uint16_t roomTypeCount = roomTypeInstance.constraints.dataGet<uint16_t>("RoomTypeCount");

                    if (addedTypes[roomTypeCur] >= roomTypeCount) {
                        roomTypeConstraintsMet = false;
                    }
                }
#pragma endregion Constraint Checking

                if (roomTypeConstraintsMet) {
                    roomFillWithType(wallGrid, structureSize, roomTypeGrid, roomCur, RoomType(roomTypeCur));
                    addedTypes[roomTypeCur]++;
                    break;
                }
            }
        }
    }

    return true;
}

