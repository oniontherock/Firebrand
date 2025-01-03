#include "StructureGenerator.hpp"

StructureGrid StructureGenerator::structureGenerate(StructureType structureType, sf::Vector2f position, sf::Vector2i structureSize) {

	StructureGrid structureGrid = StructureGrid(structureSize.x, structureSize.y, structureType, position);

	for (uint16_t x = 0; x < structureSize.x; x++) {
		for (uint16_t y = 0; y < structureSize.y; y++) {
			if ((x == 0 || x == structureSize.x - 1) && (y == 0 || y == structureSize.y - 1)) {
				structureGrid.cellSet(x, y, "Wall");
			}
		}
	}
}

