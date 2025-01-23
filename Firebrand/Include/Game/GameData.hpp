#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include "World/World Clock/WorldClock.hpp"
#include <ECS.hpp>
#include <unordered_map>
#include <Auxiliary/Cooldown.hpp>

struct GameData {
	static EntityId playerId;
	static WorldClock worldClock;
	static Cooldown physicsTimer;
	static Cooldown renderTimer;
};

#endif