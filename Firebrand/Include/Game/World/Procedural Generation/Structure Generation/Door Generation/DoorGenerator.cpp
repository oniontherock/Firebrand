#include "DoorGenerator.hpp"
#include <queue>
#include "Auxiliary/VectorMath.hpp"
#include "Auxiliary/Math.hpp"

DoorGrid2D DoorGenerator::doorsGenerate(const WallGrid2D wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, RoomRectVector& roomRectsVector) {
	//WallSectionGrid2D wallSectionGrid = wallSectionsGet(wallGrid, roomTypeGrid, structureSize);

	// iterate over all roomRects
	//for (RoomRect& roomRectCur : roomRectsVector) {

//		RoomType roomRectType = RoomDesignator::roomTypeGet(roomTypeGrid, roomRectCur);
//		
//		const RoomTypeInstance& roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(roomRectType);
//
//#pragma region Connection Types Detection
//		// allowed RoomTypes for this room to connect to.
//		// note that we don't need a forbidden connection types variable because if it was forbidden, the room wouldn't have been placed next to it in  the first place
//		std::set<RoomType> allowedConnectionTypes;
//
//		if (roomTypeInstance.constraints.dataHas("RoomConnectionsAnd")) {
//
//			// get RoomConnectionsAnd constraint
//			std::set<RoomType> roomConnections = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsAnd");
//
//			allowedConnectionTypes.insert(roomConnections.begin(), roomConnections.end());
//		}
//		// check if the room has a RoomConnectionsOr constraint
//		if (roomTypeInstance.constraints.dataHas("RoomConnectionsOr")) {
//			// get RoomConnectionsOr constraint
//			std::set<RoomType> roomConnections = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsOr");
//
//			allowedConnectionTypes.insert(roomConnections.begin(), roomConnections.end());
//		}
//#pragma endregion Connection Types Detection
//
//		std::set<sf::Vector2u, Vector2uLessThan>  possibleConnectionPoints;
//
//		// iterate over dimensions of every roomRect
//		for (uint16_t x = 0; x < roomRectCur.width; x++) {
//			for (uint16_t y = 0; y < roomRectCur.height; y++) {
//
//				bool isOnEdgeX = x == 0 || x == roomRectCur.width - 1;
//				bool isOnEdgeY = y == 0 || y == roomRectCur.height - 1;
//
//				// skip if cell is not an edge
//				if (!(isOnEdgeX || isOnEdgeY)) continue;
//
//				// skip if cell is corner cell
//				if (isOnEdgeX && isOnEdgeY) continue;
//
//				uint16_t cellX = roomRectCur.left + x;
//				uint16_t cellY = roomRectCur.top + y;
//
//				WallSectionCell& wallSection = wallSectionGrid[cellX][cellY];
//
//				// if 
//				if (wallSection.first == roomRectType && wallSection.second == roomRectType) {
//
//				}
//
//
//			}
//		}
	//}

	return DoorGrid2D();
}

