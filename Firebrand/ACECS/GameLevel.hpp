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
	GameLevel() = default;
	// inherit the constructors of BaseLevel so that the user doesn't need to specify them
	using BaseLevel::BaseLevel;

	uint32_t exampleVariable = 0;
};

using GameLevelGrid = LevelGrid<GameLevel>;

#endif