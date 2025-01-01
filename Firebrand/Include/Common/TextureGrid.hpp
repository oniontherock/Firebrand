#ifndef __TEXTURE_GRID_H__
#define __TEXTURE_GRID_H__

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "GridTexture.hpp"

typedef std::vector<GridTexture> TextureGrid1D;
typedef std::vector<TextureGrid1D> TextureGrid2D;

struct TextureGrid : Grid<GridTexture> {
	TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	
	// returns pointers to any textures in the given rect
	std::vector<GridTexture*> texturesGetInRectangle(sf::FloatRect rect);

	// draws the specified rectangle to the specified texture, note that the renderTexture's view should be set to avoid only the top left most texture('s) being drawn
	void drawRectangleToTexture(sf::FloatRect rect, sf::RenderTexture& renderTexture);
};


#endif
