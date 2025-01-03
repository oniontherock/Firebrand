#ifndef __STRUCTURE_GENERATOR_H__
#define __STRUCTURE_GENERATOR_H__

#include "StructureGrid.hpp"
#include "StructureTypes.hpp"
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>

namespace StructureGenerator {
	StructureGrid structureGenerate(StructureType structureType, sf::Vector2f structurePosition, float structureRotation, sf::Vector2i structureSize);
};

#endif