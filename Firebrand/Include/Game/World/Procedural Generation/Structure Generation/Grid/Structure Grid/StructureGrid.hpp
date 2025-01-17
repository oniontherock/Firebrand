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
	StructureGrid(uint32_t gridSizeX, uint32_t gridSizeY);
};

#endif