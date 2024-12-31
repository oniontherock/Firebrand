#include "AStarPathDrawer.hpp"

sf::RenderTexture AStarPathDrawer::pathsTexture;

void AStarPathDrawer::pathsTextureReset() {
	pathsTexture.create(4096, 4096);
	pathsTexture.clear(sf::Color::Transparent);
}

void AStarPathDrawer::pathDraw(AStarPath path) {

	float cellSize = 16.f;

	sf::RectangleShape pathCellShape;
	pathCellShape.setSize(sf::Vector2f(cellSize, cellSize));
	pathCellShape.setOrigin(cellSize / 2.f, cellSize / 2.f);
	pathCellShape.setFillColor(sf::Color(0, 0, 255, 100));

	for (uint16_t i = 0; i < path.size(); i++) {
		pathCellShape.setPosition(path[i]);
		pathsTexture.draw(pathCellShape);
	}
}


void AStarPathDrawer::cellsInvalidDraw(AStarGrid& aStarGrid) {

	sf::RectangleShape invalidCellShape;
	invalidCellShape.setSize(aStarGrid.cellsGetSize());
	invalidCellShape.setFillColor(sf::Color(0, 0, 0, 100));

	for (uint16_t x = 0; x < aStarGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < aStarGrid.gridGetSizeY(); y++) {
			if (!aStarGrid.cellGet(x, y).valid) {
				invalidCellShape.setPosition(x * aStarGrid.cellsGetSizeX(), y * aStarGrid.cellsGetSizeY());
				pathsTexture.draw(invalidCellShape);
			}
		}
	}
}
