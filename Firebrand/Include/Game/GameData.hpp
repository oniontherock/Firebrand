#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <ECS.hpp>
#include "World/World Clock/WorldClock.hpp"

struct GameData {
	static EntityId playerId;
	static WorldClock worldClock;
};

#endif