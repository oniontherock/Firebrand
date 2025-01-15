#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include "../Include/Common/TextureGrid.hpp"
#include "../Include/Game/Pathfinding/AStar/Grid/AStarGrid.hpp"
#include "../Include/Game/World/Objects/Object Grid/ObjectGrid.hpp"
#include "../Include/Game/World/Procedural Generation/Path Generation/PathGenerator.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <SFML/Graphics.hpp>
#include <World/Level.hpp>
#include <World/LevelGrid.hpp>

// example of the game level class,
// 
// note that there should generally only be one type of GameLevel,
// the reason GameLevel inherits BaseLevel and needs to be down-casted is so the user can define their own variables inside of GameLevel.
// the GameLevel and BaseLevel relationship isn't like the Component relationship where there can be many types of components,
struct GameLevel : public BaseLevel {
	// not sure whether this default constructor is necessary, but I've included it just in case.
	GameLevel();
	GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ);
	GameLevel(LevelPosition _id);

	sf::Vector2u levelSize;

	uint32_t exampleVariable = 0;




	ObjectGrid objectGrid;
	AStarGrid aStarGrid;

	PathGenerator pathGenerator;

	// texture for the background
	TextureGrid backgroundTexture;
	// texture for the paths
	TextureGrid pathsTexture;

	void pathsGenerate();
	void structuresGenerate();
	void backgroundDraw(sf::FloatRect rect, uint32_t drawIterationsMax);
	

	
	void entityMarkDrawable(EntityId id, bool isDynamic, uint16_t drawOrder = 50);

	const std::vector<std::set<EntityId>>& entitiesDrawableStaticGet();
	const std::vector<std::set<EntityId>>& entitiesDrawableDynamicGet();

private:
	std::vector<std::set<EntityId>> entitiesDrawableStatic;
	std::vector<std::set<EntityId>> entitiesDrawableDynamic;

	sf::FloatRect backgroundRect;
	sf::FloatRect backgroundRectPrev;

	std::vector<sf::Vector2f> pathPoints;

	void textureUpdateValidity(TextureGrid& texture) const;
	void textureGridsUpdateValidity(sf::FloatRect rect);

	// draws the grass on the backgroundTexture's cells that fall in the rect and haven't been drawn
	void grassDraw(sf::FloatRect rect, uint32_t drawIterationsMax);
	void pathsDraw(sf::FloatRect rect, uint32_t drawIterationsMax);
};

using GameLevelGrid = LevelGrid<GameLevel>;

#endif