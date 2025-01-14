#include "../Wall Sections/WallSectionGenerator.hpp"
#include "Auxiliary/Math.hpp"
#include "Auxiliary/NumberGenerator.hpp"
#include "Auxiliary/VectorMath.hpp"
#include "DoorGenerator.hpp"
#include <queue>

DoorGenerator::DoorGrid2D DoorGenerator::doorsGenerate(const WallSectionGenerator::WallSectionGrid2D& wallSectionGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, RoomRectVector& roomRectsVector) {

	DoorGrid2D doorGrid = DoorGrid2D(structureSize.x, DoorGrid1D(structureSize.y, false));

	// iterate over all roomRects
	for (RoomRect& roomRectCur : roomRectsVector) {

		RoomType roomRectType = RoomDesignator::roomTypeGet(roomTypeGrid, roomRectCur);
		
		const RoomTypeInstance& roomTypeInstance = RoomTypeRegistry::roomTypeInstanceGet(roomRectType);

#pragma region Connection Types Detection

		std::set<RoomType> connectionTypesAnd;
		std::set<RoomType> connectionTypesOr;

		if (roomTypeInstance.constraints.dataHas("RoomConnectionsAnd")) {
			// get RoomConnectionsAnd constraint
			connectionTypesAnd = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsAnd");
		}
		// check if the room has a RoomConnectionsOr constraint
		if (roomTypeInstance.constraints.dataHas("RoomConnectionsOr")) {
			// get RoomConnectionsOr constraint
			connectionTypesOr = roomTypeInstance.constraints.dataGet<std::set<RoomType>>("RoomConnectionsOr");
		}
#pragma endregion Connection Types Detection

		// vector of vectors of sf::Vector2u, the main vector is ordered by sectionId, and the sub vectors contain all cell positions in a section
		std::vector<std::vector<sf::Vector2u>> possibleConnectionPoints = std::vector<std::vector<sf::Vector2u>>(uint16_t(RoomType::RoomTypeSize));
		// set of neighboring RoomTypes, later used for deciding which room to connect to in connectionTypesOr
		std::set<RoomType> neighboringTypes;

		std::cout << "section id count: " << WallSectionGenerator::sectionIdCount << "\n";

		// iterate over dimensions of every roomRect
		for (uint16_t x = 0; x < roomRectCur.width; x++) {
			for (uint16_t y = 0; y < roomRectCur.height; y++) {

				bool isOnEdgeX = x <= 0 || x >= roomRectCur.width - 1;
				bool isOnEdgeY = y <= 0 || y >= roomRectCur.height - 1;

				// skip if cell is not an edge
				if (!(isOnEdgeX || isOnEdgeY)) continue;

				// skip if cell is corner cell
				if (isOnEdgeX && isOnEdgeY) continue;

				uint16_t cellX = roomRectCur.left + x;
				uint16_t cellY = roomRectCur.top + y;

				const WallSectionGenerator::WallSectionCell& wallSection = wallSectionGrid[cellX][cellY];

				// skip if not a valid id
				if (wallSection.sectionId >= UINT16_MAX) continue;

				// the RoomType of the room that this section connects to.
				// basically, if we are a bathroom, and the room to the right is a bedroom,
				// and the section says it connects a bedroom and a bathroom, we set connectionType to bedroom
				RoomType connectionType;

				// if the section's first type is our type, we set the connectionType to the second type (because it isn't us),
				// otherwise, we set the connectionType to the wallSection's first type (also because it isn't us).
				// note that this still works even if both the first and second type of the section are the same
				// (which means that the section connects us to another room of the same type)
				if (wallSection.first == roomRectType) connectionType = wallSection.second;
				else connectionType = wallSection.first;

				// skip if room type is null
				if (connectionType == RoomType::Null) continue;

				neighboringTypes.insert(connectionType);

				std::cout << "section Id: " << wallSection.sectionId << "\n";
				possibleConnectionPoints[uint16_t(connectionType)].push_back(sf::Vector2u(cellX, cellY));

			}
		}

		std::vector<RoomType> neighborsOr;
		std::set_intersection(connectionTypesOr.begin(), connectionTypesOr.end(), neighboringTypes.begin(), neighboringTypes.end(), std::back_inserter(neighborsOr));

		// points where a connection is
		std::vector<sf::Vector2u> connectionPoints;

		// index of a randomly chosen "or" connection to add a door to,
		uint16_t connectionIndexOr;
		if (!neighborsOr.empty()) connectionIndexOr = uint16_t(neighborsOr[RNGu16::getUnder(neighborsOr.size())]);
		else connectionIndexOr = UINT32_MAX;

		for (uint16_t i = 0; i < possibleConnectionPoints.size(); i++) {

			// skip if possibleConnectionPoints is empty
			if (possibleConnectionPoints[i].empty()) continue;

			bool doConnect = false;

			std::cout << "connection index: " << connectionIndexOr << "\n";

			if (i == connectionIndexOr) {
				doConnect = true;
			}
			if (connectionTypesAnd.contains(RoomType(i))) {
				doConnect = true;
			}

			if (doConnect) {
				sf::Vector2u connectionPoint = possibleConnectionPoints[i][RNGu16::getUnder(possibleConnectionPoints[i].size())];
				doorGrid[connectionPoint.x][connectionPoint.y] = true;
			}
		}

	}

	return doorGrid;
}

