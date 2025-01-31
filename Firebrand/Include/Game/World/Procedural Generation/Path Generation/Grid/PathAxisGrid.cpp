#include "PathAxisGrid.hpp"

PathAxisGrid::PathAxisGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<PathAxis>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = PathDistanceGrid2D(gridSize.x, PathDistanceGrid1D(gridSize.y, PathAxis()));
}

float PathAxisGrid::cellPathDistanceGet(CellCoordinate cellX, CellCoordinate cellY) {
	return cells[cellX][cellY].dist;
}
float PathAxisGrid::cellPathDistanceGet(CellVector cellPos) {
	return cellPathDistanceGet(cellPos.x, cellPos.y);
}
float PathAxisGrid::cellPathDistanceGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellPathDistanceGet(coordinatesWorldToCell(worldX, worldY));
}
float PathAxisGrid::cellPathDistanceGetFromWorld(WorldVector worldPos) {
	return cellPathDistanceGetFromWorld(worldPos.x, worldPos.y);
}

void PathAxisGrid::cellPathDistanceSet(CellCoordinate cellX, CellCoordinate cellY, float distance) {
	cells[cellX][cellY].dist = distance;
}
void PathAxisGrid::cellPathDistanceSet(CellVector cellPos, float distance) {
	cellPathDistanceSet(cellPos.x, cellPos.y, distance);
}
void PathAxisGrid::cellPathDistanceSetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, float distance) {
	cellPathDistanceSet(coordinatesWorldToCell(worldX, worldY), distance);
}
void PathAxisGrid::cellPathDistanceSetFromWorld(WorldVector worldPos, float distance) {
	cellPathDistanceSetFromWorld(worldPos.x, worldPos.y, distance);
}
