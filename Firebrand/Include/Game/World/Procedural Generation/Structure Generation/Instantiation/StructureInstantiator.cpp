#include "../ACECS/ECSRegistry.hpp"
#include "../ACECS/GameLevel.hpp"
#include "StructureInstantiator.hpp"
#include <ECS/Entities/EntityManager.hpp>

void StructureInstantiator::structureInstantiate(LevelPosition instantiationLevel, StructureGrid structure) {
	GameLevel* level = GameLevelGrid::levelGet(instantiationLevel);

	for (uint16_t x = 0; x < structure.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < structure.gridGetSizeY(); y++) {

			if (structure.cellGet(x, y) == "Null") continue;

			// cell's position local to the structure's origin
			sf::Vector2f cellLocalPosition = sf::Vector2f(float(x), float(y)) - (structure.gridGetSizeFull() / 2.f);

			Entity& entity = EntityManager::entityCreateAndGet(instantiationLevel, structure.cellGet(x, y).c_str());

			auto* entityComponentPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>();

			entityComponentPosition->position = structure.position + cellLocalPosition;

			auto* entityComponentRotation = entity.entityComponentGet<EntityComponents::ComponentRotation>();
			entityComponentRotation->rotation = structure.rotation;
		}
	}
}

