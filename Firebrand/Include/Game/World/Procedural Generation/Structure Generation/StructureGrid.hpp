#ifndef __STRUCTURE_GRID_H__
#define __STRUCTURE_GRID_H__

#include "../../../../Common/Grid.hpp"
#include "StructureTypes.hpp"
#include <string>
#include <vector>

typedef std::string StructureCell;

typedef std::vector<StructureCell> StructureGrid1D;
typedef std::vector<StructureGrid1D> StructureGrid2D;

// the size of cells in any StructureGrid, since there is no reason that two StructureGrids would need different cell sizes, we just use a constant value
constexpr static float structureGridCellSize = 128.f;

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