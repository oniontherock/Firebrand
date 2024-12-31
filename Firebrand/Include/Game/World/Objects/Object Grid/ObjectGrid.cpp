#include "ObjectGrid.hpp"
#include "../ObjectRegistry.hpp"
#include <algorithm>

ObjectGrid::ObjectGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<ObjectCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	ObjectGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		ObjectGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y] = ObjectCell();
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}


const std::set<EntityId>& ObjectGrid::cellIdsGet(CellCoordinate cellX, CellCoordinate cellY) {
	return cellGet(cellX, cellY).idsGet();
}
const std::set<EntityId>& ObjectGrid::cellIdsGet(CellVector cellPos) {
	return cellIdsGet(cellPos.x, cellPos.y);
}
const std::set<EntityId>& ObjectGrid::cellIdsGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellIdsGet(coordinatesWorldToCell(worldX, worldY));

}
const std::set<EntityId>& ObjectGrid::cellIdsGetFromWorld(WorldVector worldPos) {
	return cellIdsGetFromWorld(worldPos.x, worldPos.y);
}


const std::set<ObjectType> ObjectGrid::cellTypesGet(CellCoordinate cellX, CellCoordinate cellY) {

	const std::set<EntityId>& ids = cellIdsGet(cellX, cellY);
	std::set<EntityId>::iterator idsItr;

	std::set<ObjectType> types;

	for (idsItr = ids.begin(); idsItr != ids.end(); idsItr++) {
		types.insert(ObjectRegistry::entityObjectTypeGet(*idsItr));
	}

	return types;
}
const std::set<ObjectType> ObjectGrid::cellTypesGet(CellVector cellPos) {
	return cellTypesGet(cellPos.x, cellPos.y);
}
const std::set<ObjectType> ObjectGrid::cellTypesGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellTypesGet(coordinatesWorldToCell(worldX, worldY));

}
const std::set<ObjectType> ObjectGrid::cellTypesGetFromWorld(WorldVector worldPos) {
	return cellTypesGetFromWorld(worldPos.x, worldPos.y);
}
