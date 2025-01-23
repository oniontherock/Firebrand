#include "GameData.hpp"

EntityId GameData::playerId = 0;
WorldClock GameData::worldClock{};
Cooldown GameData::physicsTimer(1.f / 60.f);
Cooldown GameData::renderTimer(1.f / 120.f);
