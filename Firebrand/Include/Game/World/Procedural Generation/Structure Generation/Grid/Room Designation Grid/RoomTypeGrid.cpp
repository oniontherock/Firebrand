#include "RoomTypeGrid.hpp"

RoomTypeGrid::RoomTypeGrid(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<RoomTypeCell>::Grid(gridSizeX, gridSizeY, structureGridCellSize, structureGridCellSize)
{
	RoomTypeGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		RoomTypeGrid1D columns(gridSize.y);

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}
