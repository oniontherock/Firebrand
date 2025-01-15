#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include "World/World Clock/WorldClock.hpp"
#include <ECS.hpp>
#include <unordered_map>

struct GameData {
	static EntityId playerId;
	static WorldClock worldClock;
};

#endif