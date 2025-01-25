#ifndef __STRUCTURE_PLACEMENT_GRID_H__
#define __STRUCTURE_PLACEMENT_GRID_H__

#include "../include/Common/Grid.hpp"
#include <string>
#include <vector>

typedef bool StructurePlacementCell;

typedef std::vector<StructurePlacementCell> StructurePlacementGrid1D;
typedef std::vector<StructurePlacementGrid1D> StructurePlacementGrid2D;

struct StructurePlacementGrid : Grid<StructurePlacementCell> {
	StructurePlacementGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	StructurePlacementCell cellGet(CellCoordinate cellX, CellCoordinate cellY);
	StructurePlacementCell cellGet(CellVector cellPos);
	StructurePlacementCell cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	StructurePlacementCell cellGetFromWorld(WorldVector worldPos);
};

#endif