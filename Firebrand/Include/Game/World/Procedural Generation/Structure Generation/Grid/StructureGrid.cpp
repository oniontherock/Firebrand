#include "StructureGrid.hpp"

StructureGrid::StructureGrid(uint32_t gridSizeX, uint32_t gridSizeY, StructureType _structureType, sf::Vector2f _position, float _rotation) :
	Grid<StructureCell>::Grid(gridSizeX, gridSizeY, structureGridCellSize, structureGridCellSize)
{
	StructureGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		StructureGrid1D columns(gridSize.y);

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);

	structureType = _structureType;
	position = _position;
	rotation = _rotation;
}
