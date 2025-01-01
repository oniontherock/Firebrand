#ifndef __GRID_TEXTURE_H__
#define __GRID_TEXTURE_H__

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// render texture containing some small extra data
struct GridTexture : sf::RenderTexture {

	friend struct TextureGrid;

	sf::Vector2f positionGet();

	// returns a bool representing whether this texture has ever been drawn on
	bool hasEverDrawnGet();

	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states = sf::RenderStates::Default);
private:

	// sets hasEverDrawn to true, we do a function instead of just directly setting it so that we can added extra logic if ever needed
	void markAsDrawn();

	// bool that is set to true when the texture is drawn
	bool hasEverDrawn = false;

	sf::Vector2f position;
};


#endif