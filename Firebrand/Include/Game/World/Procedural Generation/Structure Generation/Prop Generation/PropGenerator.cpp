#include "../Path Verification/PathVerifier.hpp"
#include "PropGenerator.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <queue>
#include <set>

void PropGenerator::roomPropsGenerate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType) {
    //areaFillWithType(wallGrid, structureSize, roomTypeGrid, sf::Vector2u(roomRect.getPosition() + (roomRect.getSize() / 2)), roomType);
}

PropTypeGrid2D PropGenerator::propsGenerate(const WallGrid2D& wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector) {
    
    PropTypeGrid2D propTypeGrid = PropTypeGrid2D(structureSize.x, PropTypeGrid1D(structureSize.y, "Null"));

    for (const auto& roomCur : roomRectsVector) {

        // get RoomTypeInstance of the current room
        const RoomTypeInstance& roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(RoomType(RoomDesignator::roomTypeGet(roomTypeGrid, roomCur)));

        // iterate over every type of prop that can be placed inside this room
        for (uint16_t i = 0; i < roomTypeInstance.propTypes.size(); i++) {
            PropType propTypeCur = roomTypeInstance.propTypes[i];

            // get PropTypeInstance of the current PropType
            const PropTypeInstance& propTypeInstance = PropTypeRegistry::propTypeInstanceGet(propTypeCur);

            std::cout << "prop name: " << propTypeInstance.name << "\n";

            bool propAllowedOnEdge = true;
            bool propAllowedOnCenter = true;

            if (propTypeInstance.constraints.dataHas("IsOffEdge")) {
                propAllowedOnEdge = false;
            }
            if (propTypeInstance.constraints.dataHas("IsOffCenter")) {
                propAllowedOnCenter = false;
            }


            for (uint16_t x = 1; x < roomCur.width - 1; x++) {
                for (uint16_t y = 1; y < roomCur.height - 1; y++) {

                    bool isOnEdgeX = x <= 1 || x >= roomCur.width - 2;
                    bool isOnEdgeY = y <= 1 || y >= roomCur.height - 2;

                    // if we are on the edge, and we aren't allowed on the edge, continue
                    if ((isOnEdgeX || isOnEdgeY) && (!propAllowedOnEdge)) {
                        std::cout << "not allowed edge" << "\n"; continue;
                    }
                    // if we are on the center, and we aren't allowed on the center, continue
                    if ((!(isOnEdgeX || isOnEdgeY)) && (!propAllowedOnCenter)) {
                        std::cout << "not allowed center" << "\n"; continue;
                    }
    
                    int16_t cellX = int16_t(roomCur.left) + int16_t(x);
                    int16_t cellY = int16_t(roomCur.top) + int16_t(y);


                    if (propTypeGrid[cellX][cellY] == "Null") {
                        //if (RNGf::probability(0.1f)) continue;

                        propTypeGrid[cellX][cellY] = std::string("Prop ") + propTypeInstance.name;

                        if (RNGf::probability(0.9f)) {
                            goto nextRoom;
                        }
                    }
                }
            }
        }

    nextRoom:
        continue;
    }

    return propTypeGrid;
}
