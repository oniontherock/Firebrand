#include "TextureGrid.hpp"


TextureGrid::TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<sf::RenderTexture>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	TextureGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		TextureGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y].create(cellSize.x, cellSize.y);
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}
