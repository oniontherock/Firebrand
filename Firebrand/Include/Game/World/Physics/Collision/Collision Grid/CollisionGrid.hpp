#ifndef __COLLISION_GRID_H__
#define __COLLISION_GRID_H__

#include "../Include/Common/Grid.hpp"
#include <vector>

typedef bool CollisionCell;

typedef std::vector<CollisionCell> CollisionGrid1D;
typedef std::vector<CollisionGrid1D> CollisionGrid2D;

struct CollisionGrid : Grid<CollisionCell> {
	CollisionGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	CollisionCell cellGet(CellCoordinate cellX, CellCoordinate cellY);
	CollisionCell cellGet(CellVector cellPos);
	CollisionCell cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	CollisionCell cellGetFromWorld(WorldVector worldPos);
};

#endif