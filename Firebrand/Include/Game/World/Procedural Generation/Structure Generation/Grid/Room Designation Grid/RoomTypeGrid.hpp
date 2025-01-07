#ifndef __ROOM_TYPE_GRID_H__
#define __ROOM_TYPE_GRID_H__

#include "../include/Common/Grid.hpp"
#include "../StructureGridTypes.hpp"
#include "RoomTypeCell.hpp"
#include <string>
#include <vector>

typedef std::vector<RoomTypeCell> RoomTypeGrid1D;
typedef std::vector<RoomTypeGrid1D> RoomTypeGrid2D;

struct RoomTypeGrid : Grid<RoomTypeCell> {
	RoomTypeGrid(uint32_t gridSizeX, uint32_t gridSizeY);
};

#endif