#ifndef __STRUCTURE_GENERATOR_H__
#define __STRUCTURE_GENERATOR_H__

#include "Grid/StructureGrid.hpp"
#include "StructureTypes.hpp"
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include "Wall Generation/WallGenerator.hpp"

typedef std::vector<bool> SurroundingWallsVector;

class StructureGenerator {

public:
	// generates a building of type structureType and size, and assigns it the specified position and rotation.
	static StructureGrid structureGenerate(StructureType structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2i structureSize);
};

#endif