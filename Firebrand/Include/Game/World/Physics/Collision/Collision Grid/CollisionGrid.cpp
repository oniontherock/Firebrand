#include "CollisionGrid.hpp"
#include <algorithm>

CollisionGrid::CollisionGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<CollisionCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = CollisionGrid2D(gridSizeX, CollisionGrid1D(gridSizeY, false));
}

CollisionCell CollisionGrid::cellGet(CellCoordinate cellX, CellCoordinate cellY) {
	return cells[cellX][cellY];
}
CollisionCell CollisionGrid::cellGet(CellVector cellPos) {
	return cellGet(cellPos.x, cellPos.y);
}
CollisionCell CollisionGrid::cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellGet(coordinatesWorldToCell(worldX, worldY));
}
CollisionCell CollisionGrid::cellGetFromWorld(WorldVector worldPos) {
	return cellGetFromWorld(worldPos.x, worldPos.y);
}