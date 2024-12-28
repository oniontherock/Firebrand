#include "../Include/Game/GameData.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "GameStates.hpp"
#include <ECS/Entities/EntityManager.hpp>
#include <World/LevelUpdater.hpp>

void GameStatePlay::gameStateUpdate() {
	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);

	if (isFirstRun) {
		// create player and assign the level's playerId to the id of the newly created player
		GameData::playerId = EntityManager::entityCreate(level->levelPosition, "Player");
		Entity& player = EntityManager::entityGet(GameData::playerId);
		player.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(100, 100);
		isFirstRun = false;
	}

	EntityManager::entitiesIntangibleUpdate();
	EntityManager::entitiesQueuedUpdate();
	LevelUpdater::levelsUpdate();
}

void GameStatePause::gameStateUpdate() {
}

