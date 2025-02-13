#include "../Include/Game/GameData.hpp"
#include "GameLevel.hpp"
#include "GameStates.hpp"
#include "SaveOperations.hpp"
#include <ECS/Entities/EntityManager.hpp>
#include <GameState/GameStateHandler.hpp>
#include <Saving/SaveDirector.hpp>

// this file is for defining the SaveDirector's saving/loading functions for the game specific data.

void SaveDirector::gameDataSave() {
	// save levels
	for (uint16_t x = 0; x < levelGrid.size(); x++) {
		for (uint16_t y = 0; y < levelGrid[x].size(); y++) {
			for (uint16_t z = 0; z < levelGrid[x][y].size(); z++) {

				GameLevel& level = *static_cast<GameLevel*>(levelGrid[x][y][z].get());
				SaveHandler::objectSave<GameLevel&>(level);
			}
		}
	}

	// save entities
	for (uint32_t entityIdCur = 0; entityIdCur < EntityManager::entitiesVector.size(); entityIdCur++) {
		if (EntityManager::entityIdsSet.contains(entityIdCur)) continue;

		// save entity's id
		SaveHandler::objectSaveIgnoreErrors(entityIdCur);
		// save entity's data
		SaveHandler::objectSaveIgnoreErrors<Entity&>(EntityManager::entitiesVector[entityIdCur]);
	}
	// used for breaking the loading loop, which checks when a value exceeds another value, so obviously this will exceed that value.
	EntityId loopBreaker = MAX_ENTITIES;
	SaveHandler::objectSaveIgnoreErrors(loopBreaker);

	SaveHandler::objectSave(GameData::playerId);
}
void SaveDirector::gameDataLoad() {
	// load levels
	for (uint16_t x = 0; x < levelGrid.size(); x++) {
		for (uint16_t y = 0; y < levelGrid[x].size(); y++) {
			for (uint16_t z = 0; z < levelGrid[x][y].size(); z++) {

				GameLevel& level = *static_cast<GameLevel*>(levelGrid[x][y][z].get());
				SaveHandler::objectLoad<GameLevel&>(level);
			}
		}
	}

	// load entities
	EntityId entityIdCur;
	// initialize entityIdCur
	SaveHandler::objectLoadIgnoreErrors(entityIdCur);
	do {

		// initialize entity in EntityManager
		EntityManager::entityCreateWithId(entityIdCur);
		// get entity reference
		Entity& entity = EntityManager::entityGet(entityIdCur);
		//load entity's data
		SaveHandler::objectLoadIgnoreErrors(entity);
		// add entity to room if it's not an intangible entity
		if (entity.levelId != NoLevelPosition) {
			EntityManager::entityAddToRoom(entityIdCur, entity.levelId);
			std::cout << "added to a room" << "\n";
		}

		// load entity's id
		SaveHandler::objectLoadIgnoreErrors(entityIdCur);
	} while (entityIdCur < MAX_ENTITIES);

	SaveHandler::objectLoad(GameData::playerId);
}
