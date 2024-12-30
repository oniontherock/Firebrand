#include "AStarGrid.hpp"


typedef std::vector<AStarCell> AStarGrid1D;
typedef std::vector<AStarGrid1D> AStarGrid2D;

AStarGrid::AStarGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<AStarCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = AStarGrid2D(gridSizeX, AStarGrid1D(gridSizeY, AStarCell()));

	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cellSet(x, y, AStarCell(AStarCellPosition(x, y), cellsGetSize()));
		}
	}
}
