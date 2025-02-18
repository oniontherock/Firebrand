#ifndef __STRUCTURE_PLACER_H__
#define __STRUCTURE_PLACER_H__

#include "../ACECS/GameLevel.hpp"
#include "../Path Generation/PathGenerator.hpp"
#include <cstdint>
#include <map>
#include <set>
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

struct StructureRect : sf::FloatRect {
	float rotation;
};

namespace StructurePlacer {
	std::vector<StructureRect> structureRectsGenerate(GameLevel* level);
};

#endif