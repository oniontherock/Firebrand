#include "GameStates.hpp"

#include <ECS/Entities/EntityManager.hpp>
#include <World/LevelUpdater.hpp>

void GameStatePlay::gameStateUpdate() {
	EntityManager::entitiesIntangibleUpdate();
	EntityManager::entitiesQueuedUpdate();
	LevelUpdater::levelsUpdate();
}

void GameStatePause::gameStateUpdate() {
}

