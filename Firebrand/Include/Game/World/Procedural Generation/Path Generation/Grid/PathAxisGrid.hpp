#ifndef __PATH_AXIS_GRID_H__
#define __PATH_AXIS_GRID_H__

#include "../include/Common/Grid.hpp"
#include <string>
#include <vector>
#include "map"

struct PathAxis {
	sf::Vector2f dir;
	float dist;
};

typedef std::vector<PathAxis> PathDistanceGrid1D;
typedef std::vector<PathDistanceGrid1D> PathDistanceGrid2D;

struct PathAxisGrid : Grid<PathAxis> {
	PathAxisGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	float cellPathDistanceGet(CellCoordinate cellX, CellCoordinate cellY);
	float cellPathDistanceGet(CellVector cellPos);
	float cellPathDistanceGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	float cellPathDistanceGetFromWorld(WorldVector worldPos);

	void cellPathDistanceSet(CellCoordinate cellX, CellCoordinate cellY, float distance);
	void cellPathDistanceSet(CellVector cellPos, float distance);
	void cellPathDistanceSetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, float distance);
	void cellPathDistanceSetFromWorld(WorldVector worldPos, float distance);
};

#endif