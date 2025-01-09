#ifndef __STRUCTURE_GRID_H__
#define __STRUCTURE_GRID_H__

#include "../../Structure Types/StructureTypes.hpp"
#include "../include/Common/Grid.hpp"
#include "../StructureGridTypes.hpp"
#include "StructureCell.hpp"
#include <string>
#include <vector>

typedef std::vector<StructureCell> StructureGrid1D;
typedef std::vector<StructureGrid1D> StructureGrid2D;

struct StructureGrid : Grid<StructureCell> {
	StructureGrid(uint32_t gridSizeX, uint32_t gridSizeY, StructureType _structureType, sf::Vector2f _position, float _rotation);

	// type of structure this is
	StructureType structureType;
	// structure's position in the world
	sf::Vector2f position;
	// structure's global rotation
	float rotation;
};

#endif