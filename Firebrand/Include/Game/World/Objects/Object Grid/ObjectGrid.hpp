#ifndef __OBJECT_GRID_H__
#define __OBJECT_GRID_H__

#include "../../../../Common/Grid.hpp"
#include "../ObjectTypes.hpp"
#include "ObjectCell.hpp"
#include <vector>

typedef std::vector<ObjectCell> ObjectGrid1D;
typedef std::vector<ObjectGrid1D> ObjectGrid2D;

struct ObjectGrid : Grid<ObjectCell> {
	ObjectGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	friend std::ofstream& operator<<(std::ofstream& str, ObjectGrid& item);
	friend std::ifstream& operator>>(std::ifstream& str, ObjectGrid& item);

	const std::set<EntityId>& cellIdsGet(CellCoordinate cellX, CellCoordinate cellY);
	const std::set<EntityId>& cellIdsGet(CellVector cellPos);
	const std::set<EntityId>& cellIdsGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	const std::set<EntityId>& cellIdsGetFromWorld(WorldVector worldPos);

	const std::set<ObjectType> cellTypesGet(CellCoordinate cellX, CellCoordinate cellY);
	const std::set<ObjectType> cellTypesGet(CellVector cellPos);
	const std::set<ObjectType> cellTypesGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	const std::set<ObjectType> cellTypesGetFromWorld(WorldVector worldPos);

};

#endif