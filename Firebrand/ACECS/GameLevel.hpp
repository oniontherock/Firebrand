#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

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

	std::vector<EntityId> entitiesDrawable;
};

using GameLevelGrid = LevelGrid<GameLevel>;

#endif