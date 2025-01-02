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

	sf::Vector2f positionGet() const;
	sf::Vector2u gridPositionGet() const;

	// returns a bool representing whether this texture has ever been drawn on
	bool hasEverDrawnGet() const;
	uint32_t drawCountGet() const;
	// returns a bool representing whether this texture has ever displayed
	bool hasEverDisplayedGet() const;
	uint32_t displayCountGet() const;

	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states = sf::RenderStates::Default);
	
	void display();
	// displays the texture but does NOT increment the displayCount or hasEverDisplayed variables
	void displayInvisible();
private:

	// sets hasEverDrawn to true, and increments drawCount by one
	void drawRegister();
	// sets hasEverDisplayed to true, and increments displayCount by one
	void displayRegister();

	// amount of times this texture has been drawn on
	uint32_t drawCount = 0;
	// bool that is set to true when the texture is drawn
	bool hasEverDrawn = false;
	// amount of times this texture has been displayed
	uint32_t displayCount = 0;
	// bool that is set to true when the texture is displayed
	bool hasEverDisplayed = false;

	sf::Vector2f position;
	sf::Vector2u gridPosition;
};


#endif