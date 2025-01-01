#ifndef __TEXTURE_GRID_H__
#define __TEXTURE_GRID_H__

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// render texture containing a position in the world
struct GridTexture : sf::RenderTexture {

	friend struct TextureGrid;

	sf::Vector2f positionGet();
private:
	sf::Vector2f position;
};

typedef std::vector<GridTexture> TextureGrid1D;
typedef std::vector<TextureGrid1D> TextureGrid2D;

struct TextureGrid : Grid<GridTexture> {
	TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	
	// draws the specified rectangle to the specified texture
	void drawRectangleToTexture(sf::FloatRect rect, sf::RenderTexture& renderTexture);
};


#endif
