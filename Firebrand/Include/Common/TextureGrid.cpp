#include "TextureGrid.hpp"
#include <set>

TextureGrid::TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<GridTexture>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	TextureGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		TextureGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y].create(cellSize.x, cellSize.y);

			
			sf::Vector2f cellPosition = sf::Vector2f(float(x) * cellSizeX, float(y) * cellSizeY);

			columns[y].gridPosition = sf::Vector2u(x, y);
			columns[y].position = cellPosition;

			columns[y].setView(sf::View(sf::FloatRect(cellPosition.x, cellPosition.y, cellSizeX, cellSizeY)));
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}

struct Vector2uLessThan {
	bool operator()(const sf::Vector2u& lhs, const sf::Vector2u& rhs) const {
		if (lhs.x < rhs.x) {
			return true;
		}
		else if (lhs.x == rhs.x) {
			return lhs.y < rhs.y;
		}
		else {
			return false;
		}
	}
};

std::vector<GridTexture*> TextureGrid::texturesGetInRectangle(sf::FloatRect rect) {
	// set of sf::Vector2u where the elements are the positions of cells we've checked, used to make sure the same cell isn't checked twice
	std::set<sf::Vector2u, Vector2uLessThan> checkedCells;
	std::vector<GridTexture*> textures;

	// iterate over the rect
	for (float xOffset = 0; xOffset <= rect.width; xOffset += cellSize.x) {
		for (float yOffset = 0; yOffset <= rect.height; yOffset += cellSize.y) {
			// get the coordinate of the current pixel in the rect by offset the rect's position by the offsets
			sf::Vector2f rectCoordinate = sf::Vector2f(rect.left + xOffset, rect.top + yOffset);
			// get the cell's position
			sf::Vector2u cellPosition = coordinatesWorldToCell(rectCoordinate.x, rectCoordinate.y);

			// skip if cellPosition is not valid
			if (!cellPosIsInGrid(cellPosition)) continue;

			// check to make sure the current cell position has not been checked already
			if (!checkedCells.contains(cellPosition)) {
				checkedCells.insert(cellPosition);
				textures.push_back(&cellGet(cellPosition));
			}
		}
	}

	return textures;
}

void TextureGrid::drawRectangleToTexture(sf::FloatRect rect, sf::RenderTexture& renderTexture) {
	
	// set of sf::Vector2u where the elements are the positions of cells we've drawn, used to make sure the same cell isn't drawn twice
	std::set<sf::Vector2u, Vector2uLessThan> drawnCells;

	// iterate over the rect
	for (float xOffset = 0; xOffset <= rect.width; xOffset += cellSize.x) {
		for (float yOffset = 0; yOffset <= rect.height; yOffset += cellSize.y) {
			// get the coordinate of the current pixel in the rect by offset the rect's position by the offsets
			sf::Vector2f rectCoordinate = sf::Vector2f(rect.left + xOffset, rect.top + yOffset);
			// get the cell's position
			sf::Vector2u cellPosition = coordinatesWorldToCell(rectCoordinate.x, rectCoordinate.y);

			// skip if cellPosition is not valid
			if (!cellPosIsInGrid(cellPosition)) continue;

			// check to make sure the current cell position has not been drawn already
			if (!drawnCells.contains(cellPosition)) {
				// add the cellPosition to drawnCells
				drawnCells.insert(cellPosition);

				// get reference to the cell at the cellPosition
				GridTexture& cell = cellGet(cellPosition);
				
				// initialize sprite that will have the texture of the current cell
				sf::Sprite cellSprite;
				// set cellSprite's texture to the current cell's texture
				cellSprite.setTexture(cell.getTexture());
				// set cellSprite's position to the current cell's position
				cellSprite.setPosition(cell.positionGet());
				// draw the cellSprite to the renderTexture
				renderTexture.draw(cellSprite);
			}
		}
	}
}


