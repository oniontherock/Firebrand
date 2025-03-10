#include "../Include/Game/GameData.hpp"
#include "../Include/Game/World/Physics/Collision/Collision Processor/CollisionProcessor.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "GameStates.hpp"
#include <Auxiliary/TimeHandler.hpp>
#include <ECS/Entities/EntityManager.hpp>
#include "../Include/Game/Pathfinding/AStar/Path Creation/PathRequestManager.hpp"
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
	pathRequestsUpdate();

	EntityManager::entitiesIntangibleUpdate();
	EntityManager::entitiesQueuedUpdate();
	LevelUpdater::levelsUpdate();

	CollisionProcessor::collisionsProcess();
}

void GameStatePlay::gameStateStart() {

	GameLevel* level = GameLevelGrid::levelGet(0, 0, 0);
	if (!level->hasUpdated) {
		level->pathsGenerate();
		level->structuresGenerate();

		// create player and assign the level's playerId to the id of the newly created player
		GameData::playerId = EntityManager::entityCreate(level->levelPosition, "Player");
		Entity& player = EntityManager::entityGet(GameData::playerId);
		player.entityComponentGet<EntityComponents::ComponentPosition>()->position = level->pathGenerator.pathGet()[0]->position;

		EntityId creatureId = EntityManager::entityCreate(level->levelPosition, "Test Creature");
		Entity& creature = EntityManager::entityGet(creatureId);
		creature.entityComponentGet<EntityComponents::ComponentPosition>()->position = level->pathGenerator.pathGet()[0]->position + sf::Vector2f(256, 0);


		level->hasUpdated = true;
	}
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

void GameStatePlay::pathRequestsUpdate() {
	static Cooldown cooldown(0.1f);

	if (cooldown.updateAutoReset(float(TimeHandler::deltaRealGet()))) {
		PathRequestManager::queueProcessNext();
	}
}
