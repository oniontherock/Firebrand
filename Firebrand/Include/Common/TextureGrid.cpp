#include "TextureGrid.hpp"
#include <set>
#include <Auxiliary/ConsoleHandler.hpp>

TextureGrid::TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<GridTextureUniquePtr>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	TextureGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		TextureGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y] = GridTextureUniquePtr(nullptr);
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}


void TextureGrid::cellInitialize(CellCoordinate cellX, CellCoordinate cellY) {
	
	cells[cellX][cellY] = GridTextureUniquePtr(new GridTexture());
	
	GridTextureUniquePtr& cell = cellGet(cellX, cellY);

	if (!cell->resize(sf::Vector2u(uint32_t(cellSize.x), uint32_t(cellSize.y)))) {
		ConsoleHandler::consolePrintErr("Cell resizing failed in TextureGrid");
	}

	sf::Vector2f cellPosition = sf::Vector2f(float(cellX) * cellSize.x, float(cellY) * cellSize.y);

	cell->gridPosition = sf::Vector2u(cellX, cellY);
	cell->position = cellPosition;

	cell->setView(sf::View(sf::FloatRect(sf::Vector2f(cellPosition.x, cellPosition.y), sf::Vector2f(cellSize.x, cellSize.y))));
}
void TextureGrid::cellInitialize(CellVector cellPos) {
	cellInitialize(cellPos.x, cellPos.y);
}
void TextureGrid::cellInitializeFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	cellInitialize(coordinatesWorldToCell(worldX, worldY));
}
void TextureGrid::cellInitializeFromWorld(WorldVector worldPos) {
	cellInitializeFromWorld(worldPos.x, worldPos.y);
}

void TextureGrid::cellTerminate(CellCoordinate cellX, CellCoordinate cellY) {
	cellGet(cellX, cellY).reset();
}
void TextureGrid::cellTerminate(CellVector cellPos) {
	cellTerminate(cellPos.x, cellPos.y);
}
void TextureGrid::cellTerminateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	cellTerminate(coordinatesWorldToCell(worldX, worldY));
}
void TextureGrid::cellTerminateFromWorld(WorldVector worldPos) {
	cellTerminateFromWorld(worldPos.x, worldPos.y);
}

bool TextureGrid::cellValidate(CellCoordinate cellX, CellCoordinate cellY) {
	return cellGet(cellX, cellY).get() != nullptr;
}
bool TextureGrid::cellValidate(CellVector cellPos) {
	return cellValidate(cellPos.x, cellPos.y);
}
bool TextureGrid::cellValidateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	return cellValidate(coordinatesWorldToCell(worldX, worldY));
}
bool TextureGrid::cellValidateFromWorld(WorldVector worldPos) {
	return cellValidateFromWorld(worldPos.x, worldPos.y);
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
	for (float xOffset = 0; xOffset <= rect.size.x; xOffset += cellSize.x) {
		for (float yOffset = 0; yOffset <= rect.size.y; yOffset += cellSize.y) {
			// get the coordinate of the current pixel in the rect by offset the rect's position by the offsets
			sf::Vector2f rectCoordinate = sf::Vector2f(rect.position.x + xOffset, rect.position.y + yOffset);
			// get the cell's position
			sf::Vector2u cellPosition = coordinatesWorldToCell(rectCoordinate.x, rectCoordinate.y);

			// skip if cellPosition is not valid
			if (!cellPosIsInGrid(cellPosition)) continue;

			// skip if cell is not valid
			if (!cellValidate(cellPosition)) continue;

			// check to make sure the current cell position has not been checked already
			if (!checkedCells.contains(cellPosition)) {
				checkedCells.insert(cellPosition);
				textures.push_back(cellGet(cellPosition).get());
			}
		}
	}

	return textures;
}

std::vector<sf::Vector2u> TextureGrid::texturePositionsGetInRectangle(sf::FloatRect rect, uint8_t validity) {
	// set of sf::Vector2u where the elements are the positions of cells we've checked, used to make sure the same cell isn't checked twice
	std::set<sf::Vector2u, Vector2uLessThan> checkedCells;

	// iterate over the rect
	for (float xOffset = 0; xOffset <= rect.size.x; xOffset += cellSize.x) {
		for (float yOffset = 0; yOffset <= rect.size.y; yOffset += cellSize.y) {
			// get the coordinate of the current pixel in the rect by offset the rect's position by the offsets
			sf::Vector2f rectCoordinate = sf::Vector2f(rect.position.x + xOffset, rect.position.y + yOffset);
			// get the cell's position
			sf::Vector2u cellPosition = coordinatesWorldToCell(rectCoordinate.x, rectCoordinate.y);

			// skip if cellPosition is not valid
			if (!cellPosIsInGrid(cellPosition)) continue;

			// skip if we are checking cells validity before adding them (I.E. validity != 2) and if a cell's validity is not equal to validity
			if ((validity != 2) && (uint8_t(cellValidate(cellPosition)) != validity)) continue;

			// check to make sure the current cell position has not been checked already
			if (!checkedCells.contains(cellPosition)) {
				checkedCells.insert(cellPosition);
			}
		}
	}

	return std::vector(checkedCells.begin(), checkedCells.end());
}

void TextureGrid::drawRectangleToTexture(sf::FloatRect rect, sf::RenderTexture& renderTexture) {
	
	// set of sf::Vector2u where the elements are the positions of cells we've drawn, used to make sure the same cell isn't drawn twice
	std::set<sf::Vector2u, Vector2uLessThan> drawnCells;

	// iterate over the rect
	for (float xOffset = 0; xOffset <= rect.size.x; xOffset += cellSize.x) {
		for (float yOffset = 0; yOffset <= rect.size.y; yOffset += cellSize.y) {
			// get the coordinate of the current pixel in the rect by offset the rect's position by the offsets
			sf::Vector2f rectCoordinate = sf::Vector2f(rect.position.x + xOffset, rect.position.y + yOffset);
			// get the cell's position
			sf::Vector2u cellPosition = coordinatesWorldToCell(rectCoordinate.x, rectCoordinate.y);

			// skip if cellPosition is not valid
			if (!cellPosIsInGrid(cellPosition)) continue;

			// skip if cell is not valid
			if (!cellValidate(cellPosition)) continue;

			// check to make sure the current cell position has not been drawn already
			if (!drawnCells.contains(cellPosition)) {
				// add the cellPosition to drawnCells
				drawnCells.insert(cellPosition);

				// get reference to the cell at the cellPosition
				GridTextureUniquePtr& cell = cellGet(cellPosition);
				
				// initialize sprite that will have the texture of the current cell
				sf::Sprite cellSprite(cell->getTexture());
				// set cellSprite's position to the current cell's position
				cellSprite.setPosition(cell->positionGet());
				// draw the cellSprite to the renderTexture
				renderTexture.draw(cellSprite);
			}
		}
	}
}


