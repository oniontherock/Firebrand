#include "StructurePlacementGrid.hpp"

StructurePlacementGrid::StructurePlacementGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<StructurePlacementCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = StructurePlacementGrid2D(gridSize.x, StructurePlacementGrid1D(gridSize.y, false));
}

StructurePlacementCell StructurePlacementGrid::cellGet(CellCoordinate cellX, CellCoordinate cellY) {
	return cells[cellX][cellY];
}
StructurePlacementCell StructurePlacementGrid::cellGet(CellVector cellPos) {
	return cellGet(cellPos.x, cellPos.y);
}
StructurePlacementCell StructurePlacementGrid::cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellGet(coordinatesWorldToCell(worldX, worldY));
}
StructurePlacementCell StructurePlacementGrid::cellGetFromWorld(WorldVector worldPos) {
	return cellGetFromWorld(worldPos.x, worldPos.y);
}