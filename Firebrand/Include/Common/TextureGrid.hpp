#ifndef __TEXTURE_GRID_H__
#define __TEXTURE_GRID_H__

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "GridTexture.hpp"

typedef std::unique_ptr<GridTexture> GridTextureUniquePtr;

typedef std::vector<GridTextureUniquePtr> TextureGrid1D;
typedef std::vector<TextureGrid1D> TextureGrid2D;

struct TextureGrid : Grid<GridTextureUniquePtr> {
	TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	
	// sets the GridTextureUniquePtr at the given cell and initializes it's data
	void cellInitialize(CellCoordinate cellX, CellCoordinate cellY);
	// sets the GridTextureUniquePtr at the given cell and initializes it's data
	void cellInitialize(CellVector cellPos);
	// sets the GridTextureUniquePtr at the given cell and initializes it's data
	void cellInitializeFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	// sets the GridTextureUniquePtr at the given cell and initializes it's data
	void cellInitializeFromWorld(WorldVector worldPos);

	// resets the GridTextureUniquePtr at the given cell
	void cellTerminate(CellCoordinate cellX, CellCoordinate cellY);
	// resets the GridTextureUniquePtr at the given cell
	void cellTerminate(CellVector cellPos);
	// resets the GridTextureUniquePtr at the given cell
	void cellTerminateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	// resets the GridTextureUniquePtr at the given cell
	void cellTerminateFromWorld(WorldVector worldPos);

	// returns whether the cell is a nullptr or not, returns true if not a nullptr
	bool cellValidate(CellCoordinate cellX, CellCoordinate cellY);
	// returns whether the cell is a nullptr or not, returns true if not a nullptr
	bool cellValidate(CellVector cellPos);
	// returns whether the cell is a nullptr or not, returns true if not a nullptr
	bool cellValidateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	// returns whether the cell is a nullptr or not, returns true if not a nullptr
	bool cellValidateFromWorld(WorldVector worldPos);

	// returns pointers to any textures in the given rect, does not return invalid textures
	std::vector<GridTexture*> texturesGetInRectangle(sf::FloatRect rect);
	// returns the cell position of any textures in the given rect,
	// if validity == 0, return any invalid cells.
	// if validity == 1, return any valid cells.
	// if validity == 2, return any cells.
	std::vector<sf::Vector2u> texturePositionsGetInRectangle(sf::FloatRect rect, uint8_t validity = 2);

	// draws the specified rectangle to the specified texture, note that the renderTexture's view should be set to avoid only the top left most texture('s) being drawn
	void drawRectangleToTexture(sf::FloatRect rect, sf::RenderTexture& renderTexture);
};


#endif
