#include "../Wall Sections/WallSectionGenerator.hpp"
#include "Auxiliary/Math.hpp"
#include "Auxiliary/NumberGenerator.hpp"
#include "Auxiliary/VectorMath.hpp"
#include "DoorGenerator.hpp"
#include <queue>

void DoorGenerator::roomDoorsGenerate(const WallSectionGenerator::WallSectionGrid2D& wallSectionGrid, RoomTypeGrid& roomTypeGrid, DoorGrid2D& doorGrid, RoomRectVector& roomRectsVector) {

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

		//std::cout << "section id count: " << WallSectionGenerator::sectionIdCount << "\n";

		// iterate over dimensions of every roomRect
		for (uint16_t x = 0; x < roomRectCur.width; x++) {
			for (uint16_t y = 0; y < roomRectCur.height; y++) {

				bool isOnEdgeX = x <= 0 || x >= roomRectCur.width - 1;
				bool isOnEdgeY = y <= 0 || y >= roomRectCur.height - 1;

				// skip if cell is not an edge
				if (!(isOnEdgeX || isOnEdgeY)) continue;

				// skip if cell is corner cell
				if (isOnEdgeX && isOnEdgeY) continue;


				uint16_t cellX = uint16_t(roomRectCur.left) + x;
				uint16_t cellY = uint16_t(roomRectCur.top) + y;

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

				possibleConnectionPoints[uint16_t(connectionType)].push_back(sf::Vector2u(cellX, cellY));

			}
		}

		std::vector<RoomType> neighborsOr;
		std::set_intersection(connectionTypesOr.begin(), connectionTypesOr.end(), neighboringTypes.begin(), neighboringTypes.end(), std::back_inserter(neighborsOr));

		// points where a connection is
		std::vector<sf::Vector2u> connectionPoints;

		// index of a randomly chosen "or" connection to add a door to,
		uint16_t connectionIndexOr;
		if (!neighborsOr.empty()) connectionIndexOr = uint16_t(neighborsOr[RNGu16::getUnder(uint16_t(neighborsOr.size()))]);
		else connectionIndexOr = UINT16_MAX;

		for (uint16_t i = 0; i < possibleConnectionPoints.size(); i++) {

			// skip if possibleConnectionPoints is empty
			if (possibleConnectionPoints[i].empty()) continue;

			bool doConnect = false;

			if (i == connectionIndexOr) {
				doConnect = true;
			}
			if (connectionTypesAnd.contains(RoomType(i))) {
				doConnect = true;
			}

			if (doConnect) {
				sf::Vector2u connectionPoint = possibleConnectionPoints[i][RNGu16::getUnder(uint16_t(possibleConnectionPoints[i].size()))];
				doorGrid[connectionPoint.x][connectionPoint.y] = true;
			}
		}
	}
}

sf::Vector2u DoorGenerator::exteriorDoorRightSideGet(RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize) {
	std::vector<sf::Vector2u> possibleRightSideConnectionPoints;
	uint16_t x = structureSize.x - 1;
	for (uint16_t y = 0; y < structureSize.y; y++) {

		if (roomTypeGrid.cellGet(x - 1, y).type == RoomType::Hallway) {
			possibleRightSideConnectionPoints.push_back(sf::Vector2u(x, y));
		}
	}

	if (possibleRightSideConnectionPoints.size() <= 0) return sf::Vector2u(9999, 9999);

	return possibleRightSideConnectionPoints[RNGu16::getUnder(possibleRightSideConnectionPoints.size())];
}

void DoorGenerator::exteriorDoorsGenerate(RoomTypeGrid& roomTypeGrid, DoorGrid2D& doorGrid, const sf::Vector2u structureSize, const uint16_t desiredDoorCount) {
	
	
	sf::Vector2u rightSideConnectionPoint = exteriorDoorRightSideGet(roomTypeGrid, structureSize);

	std::vector<sf::Vector2u> possibleConnectionPoints = std::vector<sf::Vector2u>(uint16_t(RoomType::RoomTypeSize));

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			bool isOnEdgeX = x <= 0 || x >= structureSize.x - 1;
			bool isOnEdgeY = y <= 0 || y >= structureSize.y - 1;

			// skip if cell is not an edge
			if (!(isOnEdgeX || isOnEdgeY)) continue;

			// skip if cell is corner cell
			if ((x <= 1 || x >= structureSize.x - 2) && (y <= 1 || y >= structureSize.y - 2)) continue;

			for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
				for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

					int16_t offsettedX = int16_t(x) + offsetX;
					int16_t offsettedY = int16_t(y) + offsetY;

					// skip if even (I.E. not a cardinal direction)
					if (Mathi32::isEven(abs(offsetX + offsetY))) continue;

					// skip if pos is not in grid
					if (!roomTypeGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

					// skip if room type is not hallway
					if (roomTypeGrid.cellGet(offsettedX, offsettedY).type != RoomType::Hallway) continue;

					possibleConnectionPoints.push_back(sf::Vector2u(x, y));
					break;

				}
			}
		}
	}

	// shuffle possibleConnectionPoints
	for (int16_t i = int16_t(possibleConnectionPoints.size()) - 1; i >= 0; i--) {

		// generate the random index 
		uint16_t j = RNGu16::getUnder(i + 1);

		sf::Vector2u temp = possibleConnectionPoints[i];
		possibleConnectionPoints[i] = possibleConnectionPoints[j];
		possibleConnectionPoints[j] = temp;
	}

	const float minDist = std::min((float(structureSize.x) * 0.3333333f), (float(structureSize.y) * 0.3333333f));
	

	// iterate over possibleConnectionPoints and remove any points that are too close
	for (int16_t i = int16_t(possibleConnectionPoints.size() - 1); i >= 0; i--) {
		for (int16_t j = int16_t(possibleConnectionPoints.size() - 1); j >= 0; j--) {

			// skip if i equals j
			if (i == j) continue;

			float axisLenSqrd = Vector2iMath::distSqrd(sf::Vector2i(possibleConnectionPoints[i]), sf::Vector2i(possibleConnectionPoints[j]));

			if (axisLenSqrd < (minDist * minDist)) {

				if (possibleConnectionPoints[i] == rightSideConnectionPoint) {
					possibleConnectionPoints.erase(possibleConnectionPoints.begin() + j);
					break;
				}
				possibleConnectionPoints.erase(possibleConnectionPoints.begin() + i);

				break;
			}
		}
	}
	
	if (possibleConnectionPoints.size() <= 0) return;

	for (uint16_t i = 0; i < desiredDoorCount; i++) {
	
		sf::Vector2u doorPos;
		if (i == 0) {
			doorPos = rightSideConnectionPoint;
		}
		else {
			doorPos = possibleConnectionPoints.back();
			possibleConnectionPoints.pop_back();
		}

		doorGrid[doorPos.x][doorPos.y] = true;
	}
}
bool DoorGenerator::doorGenerationPossible(RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize) {
	
	uint16_t x = structureSize.x - 1;
	for (uint16_t y = 0; y < structureSize.y; y++) {
		
		if (roomTypeGrid.cellGet(x - 1, y).type == RoomType::Hallway) {
			return true;
		}
	}

	return false;
}

DoorGenerator::DoorGrid2D DoorGenerator::doorsGenerate(const WallSectionGenerator::WallSectionGrid2D& wallSectionGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, RoomRectVector& roomRectsVector) {

	DoorGrid2D doorGrid = DoorGrid2D(structureSize.x, DoorGrid1D(structureSize.y, false));

	roomDoorsGenerate(wallSectionGrid, roomTypeGrid, doorGrid, roomRectsVector);
	exteriorDoorsGenerate(roomTypeGrid, doorGrid, structureSize, RNGu16::getRange(1, 4));

	return doorGrid;
}

