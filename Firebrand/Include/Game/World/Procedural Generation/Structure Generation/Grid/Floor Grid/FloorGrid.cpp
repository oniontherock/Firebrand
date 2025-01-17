#include "FloorGrid.hpp"

FloorGrid::FloorGrid(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<FloorCell>::Grid(gridSizeX, gridSizeY, structureGridCellSize / 2.f, structureGridCellSize / 2.f)
{
	cells = FloorGrid2D(gridSizeX, FloorGrid1D(gridSizeY, "Null"));
}
