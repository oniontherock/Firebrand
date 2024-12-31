#ifndef __TEXTURE_GRID_H__
#define __TEXTURE_GRID_H__

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

typedef std::vector<sf::RenderTexture> TextureGrid1D;
typedef std::vector<TextureGrid1D> TextureGrid2D;

struct TextureGrid : Grid<sf::RenderTexture> {
	TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
};

#endif
