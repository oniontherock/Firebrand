#ifndef __FLOOR_GRID_H__
#define __FLOOR_GRID_H__

#include "../../Structure Types/StructureTypes.hpp"
#include "../include/Common/Grid.hpp"
#include "../StructureGridTypes.hpp"
#include <string>
#include <vector>

typedef std::string FloorCell;

typedef std::vector<FloorCell> FloorGrid1D;
typedef std::vector<FloorGrid1D> FloorGrid2D;

struct FloorGrid : Grid<FloorCell> {
	FloorGrid(uint32_t gridSizeX, uint32_t gridSizeY);
};

#endif