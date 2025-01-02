#include "GridTexture.hpp"


void GridTexture::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
	sf::RenderTexture::draw(drawable, states);
	drawRegister();
}
void GridTexture::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states) {
	sf::RenderTexture::draw(vertices, vertexCount, type, states);
	drawRegister();
}
void GridTexture::draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states) {
	sf::RenderTexture::draw(vertexBuffer, states);
	drawRegister();
}
void GridTexture::draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states) {
	sf::RenderTexture::draw(vertexBuffer, firstVertex, vertexCount, states);
	drawRegister();
}

void GridTexture::display() {
	sf::RenderTexture::display();
	displayRegister();
}
void GridTexture::displayInvisible() {
	sf::RenderTexture::display();
}


sf::Vector2f GridTexture::positionGet() const {
	return position;
}
sf::Vector2u GridTexture::gridPositionGet() const {
	return gridPosition;
}

bool GridTexture::hasEverDrawnGet() const {
	return hasEverDrawn;
}
uint32_t GridTexture::drawCountGet() const {
	return drawCount;
}

bool GridTexture::hasEverDisplayedGet() const {
	return hasEverDisplayed;
}
uint32_t GridTexture::displayCountGet() const {
	return displayCount;
}

void GridTexture::drawRegister() {
	hasEverDrawn = true;
	drawCount++;
}
void GridTexture::displayRegister() {
	hasEverDisplayed = true;
	displayCount++;
}


