#include "OcclusionGrid.hpp"
#include <algorithm>

OcclusionGrid::OcclusionGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<OcclusionCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = OcclusionGrid2D(gridSizeX, OcclusionGrid1D(gridSizeY, false));
}

OcclusionCell OcclusionGrid::cellGet(CellCoordinate cellX, CellCoordinate cellY) {
	return cells[cellX][cellY];
}
OcclusionCell OcclusionGrid::cellGet(CellVector cellPos) {
	return cellGet(cellPos.x, cellPos.y);
}
OcclusionCell OcclusionGrid::cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellGet(coordinatesWorldToCell(worldX, worldY));
}
OcclusionCell OcclusionGrid::cellGetFromWorld(WorldVector worldPos) {
	return cellGetFromWorld(worldPos.x, worldPos.y);
}