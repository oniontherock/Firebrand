#ifndef __STRUCTURE_GENERATOR_H__
#define __STRUCTURE_GENERATOR_H__

#include "Grid/Structure Grid/StructureGrid.hpp"
#include "Structure Types/StructureTypes.hpp"
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include "Wall Generation/WallGenerator.hpp"
#include "Structure.hpp"

class StructureGenerator {

public:
	// generates a building of type structureType and size, and assigns it the specified position and rotation.
	static Structure structureGenerate(StructureTypeBase* structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2u structureSize);
};

#endif