#include "../Path Verification/PathVerifier.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include "PropGenerator.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <bitset>
#include <queue>
#include <set>

void PropGenerator::roomPropsGenerate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType) {
    //areaFillWithType(wallGrid, structureSize, roomTypeGrid, sf::Vector2u(roomRect.getPosition() + (roomRect.getSize() / 2)), roomType);
}

PropTypeGrid2D PropGenerator::propsGenerate(const WallGrid2D& wallGrid, const DoorGenerator::DoorGrid2D& doorGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector) {
    
    PropTypeGrid2D propTypeGrid = PropTypeGrid2D(structureSize.x, PropTypeGrid1D(structureSize.y, PropCell("Null", 0, sf::Vector2f(0, 0))));

    for (const auto& roomCur : roomRectsVector) {

        // get RoomTypeInstance of the current room
        const RoomTypeInstance& roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(RoomType(RoomDesignator::roomTypeGet(roomTypeGrid, roomCur)));

        // iterate over every type of prop that can be placed inside this room
        for (uint16_t i = 0; i < roomTypeInstance.propTypes.size(); i++) {
            PropType propTypeCur = roomTypeInstance.propTypes[i];

            // get PropTypeInstance of the current PropType
            const PropTypeInstance& propTypeInstance = PropTypeRegistry::propTypeInstanceGet(propTypeCur);

            bool isOffEdge = propTypeInstance.constraints.dataHas("IsOffEdge");
            bool isOffCenter = propTypeInstance.constraints.dataHas("IsOffCenter");
            bool isWallAdjacent = propTypeInstance.constraints.dataHas("IsWallAdjacent");
            bool isFlushWithWall = propTypeInstance.constraints.dataHas("IsFlushWithWall");

            std::vector<sf::Vector2u> allowedPoints;

            for (uint16_t x = 1; x < roomCur.width - 1; x++) {
                for (uint16_t y = 1; y < roomCur.height - 1; y++) {

                    bool isOnEdgeX = x <= 1 || x >= roomCur.width - 2;
                    bool isOnEdgeY = y <= 1 || y >= roomCur.height - 2;

                    bool isOnEdge = isOnEdgeX || isOnEdgeY;

                    // if we are on the edge, and we aren't allowed on the edge, continue
                    if (isOnEdge && isOffEdge) continue;
                    // if we are on the center, and we aren't allowed on the center, continue
                    if (!isOnEdge && isOffCenter) continue;

                    int16_t cellX = int16_t(roomCur.left) + int16_t(x);
                    int16_t cellY = int16_t(roomCur.top) + int16_t(y);

                    sf::Vector2u cell(cellX, cellY);

                    if (!((wallGrid[int16_t(cell.x + 1)][cell.y] && (!doorGrid[int16_t(cell.x + 1)][cell.y])) ||
                        (wallGrid[cell.x][int16_t(cell.y - 1)] && (!doorGrid[cell.x][int16_t(cell.y - 1)])) ||
                        (wallGrid[int16_t(cell.x - 1)][cell.y] && (!doorGrid[int16_t(cell.x - 1)][cell.y])) ||
                        (wallGrid[cell.x][int16_t(cell.y + 1)] && (!doorGrid[cell.x][int16_t(cell.y + 1)]))) &&
                        isWallAdjacent
                        ) {
                        continue;
                    }
                    allowedPoints.push_back(sf::Vector2u(uint16_t(cellX), uint16_t(cellY)));
                }
            }

            if (allowedPoints.size() <= 0) {
                continue;
            }

            sf::Vector2u cell = allowedPoints[uint16_t(RNGu16::getUnder(allowedPoints.size()))];

            // states of the walls in the 4 cardinal directions, ordered starting from the right going counter-clockwise
            std::bitset<4> wallStates;

            wallStates[0] = wallGrid[int16_t(cell.x + 1)][cell.y] && (!doorGrid[int16_t(cell.x + 1)][cell.y]);
            wallStates[1] = wallGrid[cell.x][int16_t(cell.y - 1)] && (!doorGrid[cell.x][int16_t(cell.y - 1)]);
            wallStates[2] = wallGrid[int16_t(cell.x - 1)][cell.y] && (!doorGrid[int16_t(cell.x - 1)][cell.y]);
            wallStates[3] = wallGrid[cell.x][int16_t(cell.y + 1)] && (!doorGrid[cell.x][int16_t(cell.y + 1)]);

            if (propTypeGrid[cell.x][cell.y].name == "Null") {

                propTypeGrid[cell.x][cell.y].name = std::string("Prop ") + propTypeInstance.name;

                if (isFlushWithWall) {
                    if (wallStates[0]) {
                        propTypeGrid[cell.x][cell.y].rotation = -Mathf::PI;
                    }
                    else if (wallStates[1]) {
                        propTypeGrid[cell.x][cell.y].rotation = Mathf::PI / 2.f;
                    }
                    else if (wallStates[2]) {
                        propTypeGrid[cell.x][cell.y].rotation = 0;
                    }
                    else if (wallStates[3]) {
                        propTypeGrid[cell.x][cell.y].rotation = -Mathf::PI / 2.f;
                    }

                    propTypeGrid[cell.x][cell.y].offset = Vector2fMath::rotate(sf::Vector2f(propTypeInstance.distToBeWallFlush, 0), propTypeGrid[cell.x][cell.y].rotation + Mathf::PI);
                }

                if (RNGf::probability(0.9f)) {
                    goto nextRoom;
                }
            }
        }

    nextRoom:
        continue;
    }

    return propTypeGrid;
}
