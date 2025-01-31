#include "StructurePlacementChanceGrid.hpp"

StructurePlacementChanceGrid::StructurePlacementChanceGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<StructurePlacementChanceCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = StructurePlacementChanceGrid2D(gridSize.x, StructurePlacementChanceGrid1D(gridSize.y, StructurePlacementChanceCell()));
}

PlacementChance StructurePlacementChanceGrid::cellPlacementChanceGet(CellCoordinate cellX, CellCoordinate cellY, StructureType type) {
	return cells[cellX][cellY].placementChances[type];
}
PlacementChance StructurePlacementChanceGrid::cellPlacementChanceGet(CellVector cellPos, StructureType type) {
	return cellPlacementChanceGet(cellPos.x, cellPos.y, type);
}
PlacementChance StructurePlacementChanceGrid::cellPlacementChanceGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, StructureType type) {
	return cellPlacementChanceGet(coordinatesWorldToCell(worldX, worldY), type);
}
PlacementChance StructurePlacementChanceGrid::cellPlacementChanceGetFromWorld(WorldVector worldPos, StructureType type) {
	return cellPlacementChanceGetFromWorld(worldPos.x, worldPos.y, type);
}

void StructurePlacementChanceGrid::cellPlacementChanceSet(CellCoordinate cellX, CellCoordinate cellY, StructureType type, PlacementChance chance) {
	cells[cellX][cellY].placementChances[type] = chance;
}
void StructurePlacementChanceGrid::cellPlacementChanceSet(CellVector cellPos, StructureType type, PlacementChance chance) {
	cellPlacementChanceSet(cellPos.x, cellPos.y, type, chance);
}
void StructurePlacementChanceGrid::cellPlacementChanceSetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, StructureType type, PlacementChance chance) {
	cellPlacementChanceSet(coordinatesWorldToCell(worldX, worldY), type, chance);
}
void StructurePlacementChanceGrid::cellPlacementChanceSetFromWorld(WorldVector worldPos, StructureType type, PlacementChance chance) {
	cellPlacementChanceSetFromWorld(worldPos.x, worldPos.y, type, chance);
}
