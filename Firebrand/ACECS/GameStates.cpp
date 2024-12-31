#include "../Include/Game/GameData.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "GameStates.hpp"
#include <Auxiliary/TimeHandler.hpp>
#include <ECS/Entities/EntityManager.hpp>
#include <Graphics/WindowHolder.hpp>
#include <World/LevelUpdater.hpp>

void GameStatePlay::gameStateUpdate() {

	static int timeState = 1;
	if (InputInterface::inputGetActive("Toggle Timescale")) {

		timeState = (timeState + 1) % 3;
	}
	switch (timeState) {
	case 0:
		TimeHandler::timescale = 0.5f;
		break;
	case 1:
		TimeHandler::timescale = 1.f;
		break;
	case 2:
		TimeHandler::timescale = 10.f;
		break;
	}

	if (isFirstRun) {
		isFirstRun = false;
		gameStateStart();
	}
	
	worldClockUpdate();

	EntityManager::entitiesIntangibleUpdate();
	EntityManager::entitiesQueuedUpdate();
	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);
	//level->pathsGenerate();
	level->backgroundDraw();

	// create player and assign the level's playerId to the id of the newly created player
	GameData::playerId = EntityManager::entityCreate(level->levelPosition, "Player");
	Entity& player = EntityManager::entityGet(GameData::playerId);
	player.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(100, 100);

	// create player and assign the level's playerId to the id of the newly created player
	EntityId testId = EntityManager::entityCreate(level->levelPosition, "Test Object");
	Entity& test = EntityManager::entityGet(testId);
	test.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(256, 100);
}

void GameStatePlay::worldClockUpdate() {
	GameData::worldClock.update(TimeHandler::timeSimulatedGet());

	if (GameData::worldClock.dayCurrentGet() >= GameData::worldClock.dayCountMax) {
		GameStateHandler::gameStateForceSet(GameStateType::Lose);
	}
}

void GameStatePause::gameStateUpdate() {
	WindowHolder::windowGet().close();
}


void GameStateWin::gameStateUpdate() {

}

void GameStateLose::gameStateUpdate() {
	if (exitCooldown.updateAutoReset(float(TimeHandler::deltaSimulatedGet()))) {
		WindowHolder::windowGet().close();
	}
}

