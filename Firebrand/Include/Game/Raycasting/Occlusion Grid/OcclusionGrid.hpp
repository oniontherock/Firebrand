#ifndef __OCCLUSION_GRID_H__
#define __OCCLUSION_GRID_H__

#include "../Include/Common/Grid.hpp"
#include <vector>

typedef bool OcclusionCell;

typedef std::vector<OcclusionCell> OcclusionGrid1D;
typedef std::vector<OcclusionGrid1D> OcclusionGrid2D;

struct OcclusionGrid : Grid<OcclusionCell> {
	OcclusionGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	OcclusionCell cellGet(CellCoordinate cellX, CellCoordinate cellY);
	OcclusionCell cellGet(CellVector cellPos);
	OcclusionCell cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	OcclusionCell cellGetFromWorld(WorldVector worldPos);
};

#endif