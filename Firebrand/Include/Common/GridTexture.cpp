#include "GridTexture.hpp"



sf::Vector2f GridTexture::positionGet() {
	return position;
}

void GridTexture::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
	sf::RenderTexture::draw(drawable, states);
	markAsDrawn();
}
void GridTexture::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states) {
	sf::RenderTexture::draw(vertices, vertexCount, type, states);
	markAsDrawn();
}
void GridTexture::draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states) {
	sf::RenderTexture::draw(vertexBuffer, states);
	markAsDrawn();
}
void GridTexture::draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states) {
	sf::RenderTexture::draw(vertexBuffer, firstVertex, vertexCount, states);
	markAsDrawn();
}

bool GridTexture::hasEverDrawnGet() {
	return hasEverDrawn;
}
void GridTexture::markAsDrawn() {
	hasEverDrawn = true;
}

