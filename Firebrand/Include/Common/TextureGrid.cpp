#include "TextureGrid.hpp"

sf::Vector2f GridTexture::positionGet() {
	return position;
}

TextureGrid::TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<GridTexture>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	TextureGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		TextureGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y].create(cellSize.x, cellSize.y);
			columns[y].position = sf::Vector2f(float(x) * cellSizeX, float(y) * cellSizeY);
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}

void TextureGrid::drawRectangleToTexture(sf::FloatRect rect, sf::RenderTexture& renderTexture) {
	
	// vector of sf::Vector2u where the elements are the positions of cells we've drawn, used to make sure the same cell isn't drawn twice
	std::vector<sf::Vector2u> drawnCells;

	// iterate over the rect
	for (float xOffset = 0; xOffset <= rect.width; xOffset += cellSize.x / 2.f) {
		for (float yOffset = 0; yOffset <= rect.height; yOffset += cellSize.y / 2.f) {
			// get the coordinate of the current pixel in the rect by offset the rect's position by the offsets
			sf::Vector2f rectCoordinate = sf::Vector2f(rect.left + xOffset, rect.top + yOffset);
			// get the cell's position
			sf::Vector2u cellPosition = coordinatesWorldToCell(rectCoordinate.x, rectCoordinate.y);

			// skip if cellPosition is not valid
			if (!cellPosIsInGrid(cellPosition)) continue;

			// check to make sure the current cell position has not been drawn already
			if (std::find(drawnCells.begin(), drawnCells.end(), cellPosition) == drawnCells.end()) {
				// add the cellPosition to drawnCells
				drawnCells.push_back(cellPosition);

				// get reference to the cell at the cellPosition
				GridTexture& cell = cellGet(cellPosition);
				
				// initialize sprite that will have the texture of the current cell
				sf::Sprite cellSprite;
				// set cellSprite's texture to the current cell's texture
				cellSprite.setTexture(cell.getTexture());
				// set cellSprite's position to the current cell's position relative to the rect
				cellSprite.setPosition(cell.positionGet() - rect.getPosition());
				// draw the cellSprite to the renderTexture
				renderTexture.draw(cellSprite);
			}
		}
	}
}


