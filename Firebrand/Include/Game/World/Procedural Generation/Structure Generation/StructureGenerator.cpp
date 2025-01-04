#include "StructureGenerator.hpp"
#include <Auxiliary/Math.hpp>

StructureGrid StructureGenerator::structureGenerate(StructureType structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2i structureSize) {

	std::vector<std::vector<bool>> wallGrid = std::vector<std::vector<bool>>(structureSize.x, std::vector<bool>(structureSize.y, false));

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			if ((x == 0 || x == structureSize.x - 1) || (y == 0 || y == structureSize.y - 1)) {
				wallGrid[x][y] = true;
			}
		}
	}

	StructureGrid structureGrid = StructureGrid(structureSize.x, structureSize.y, structureType, structurePosition, structureRotation);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			if (!wallGrid[x][y]) continue;

			std::vector<bool> wallStates(9, false);

			for (int16_t offsetX = -1; offsetX <= 1; offsetX++) {
				for (int16_t offsetY = -1; offsetY <= 1; offsetY++) {

					// if the offset is even it means it's either a diagonal, or (0, 0), which we don't want, so skip if it is
					if (Mathi16::isEven(abs(offsetX + offsetY))) continue;

					int16_t offsettedX = x + offsetX;
					int16_t offsettedY = y + offsetY;

					if (!structureGrid.cellPosIsInGrid(offsettedX, offsettedY)) continue;

					uint16_t flattenedIndex = uint16_t(((offsetY + 1) * 3) + (offsetX + 1));

					wallStates[flattenedIndex] = wallGrid[offsettedX][offsettedY];
				}
			}

			enum WallType {
				None,
				Single,
				Straight,
				Corner,
				JunctionT,
				JunctionPlus,
			};

			WallType wallType;
			float wallRotation = 0.f;

			// check all
			if (wallStates[1] && wallStates[3] && wallStates[5] && wallStates[7]) {
				wallType = JunctionPlus;
			}
			/// Check T shapes
			// check right facing T
			else if (wallStates[1] && wallStates[5] && wallStates[7]) {
				wallType = JunctionT;
				wallRotation = 0;
			}
			// check up facing T
			else if (wallStates[1] && wallStates[3] && wallStates[5]) {
				wallType = JunctionT;
				wallRotation = -Mathf::PI / 2.f;
			}
			// check left facing T
			else if (wallStates[1] && wallStates[3] && wallStates[7]) {
				wallType = JunctionT;
				wallRotation = Mathf::PI;
			}
			// check down facing T
			else if (wallStates[3] && wallStates[5] && wallStates[7]) {
				wallType = JunctionT;
				wallRotation = Mathf::PI / 2.f;
			}
			/// End of check T shapes
			/// Check straights
			// horizontal
			else if (wallStates[3] && wallStates[5]) {
				wallType = Straight;
			}
			// vertical
			else if (wallStates[1] && wallStates[7]) {
				wallType = Straight;
				wallRotation = Mathf::PI / 2.f;
			}
			/// End of check straights
			/// Check corners
			// bottom and right
			else if (wallStates[5] && wallStates[7]) {
				wallType = Corner;
				wallRotation = 0;
			}
			// bottom and left
			else if (wallStates[3] && wallStates[7]) {
				wallType = Corner;
				wallRotation = Mathf::PI / 2.f;
			}
			// top and left
			else if (wallStates[1] && wallStates[3]) {
				wallType = Corner;
				wallRotation = Mathf::PI;
			}
			// top and right
			else if (wallStates[1] && wallStates[5]) {
				wallType = Corner;
				wallRotation = -Mathf::PI / 2.f;
			}
			/// End of check corners
			/// Check singles
			// right
			else if (wallStates[5]) {
				wallType = Single;
				wallRotation = 0;
			}
			// top
			else if (wallStates[1]) {
				wallType = Single;
				wallRotation = -Mathf::PI / 2.f;
			}
			// left
			else if (wallStates[3]) {
				wallType = Single;
				wallRotation = Mathf::PI;
			}
			// down
			else if (wallStates[7]) {
				wallType = Single;
				wallRotation = Mathf::PI / 2.f;
			}
			/// End of check singles
			else {
				wallType = None;
			}

			StructureCell cell = StructureCell("Null", wallRotation);

			switch (wallType) {
			case None:
				break;
			case Single:
				cell.type = "Wall Single";
				break;
			case Straight:
				cell.type = "Wall Straight";
				break;
			case Corner:
				cell.type = "Wall Corner";
				break;
			case JunctionT:
				cell.type = "Wall Junction T";
				break;
			case JunctionPlus:
				cell.type = "Wall Junction Plus";
				break;
			}

			structureGrid.cellSet(x, y, cell);
			
		}
	}

	return structureGrid;
}

