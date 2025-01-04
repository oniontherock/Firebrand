#include "../ACECS/ECSRegistry.hpp"
#include "../ACECS/GameLevel.hpp"
#include "StructureInstantiator.hpp"
#include <bitset>
#include <ECS/Entities/EntityManager.hpp>

void StructureInstantiator::structureInstantiate(LevelPosition instantiationLevel, StructureGrid structure) {
	GameLevel* level = GameLevelGrid::levelGet(instantiationLevel);

	for (uint16_t x = 0; x < structure.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < structure.gridGetSizeY(); y++) {

			StructureCell cell = structure.cellGet(x, y);

			if (cell.type == "Null") continue;

			sf::Vector2f cellPosition = sf::Vector2f(0, 0);

			// cell's position local to the structure's origin
			sf::Vector2f cellPositionLocal = sf::Vector2f(float(x) * structureGridCellSize, float(y) * structureGridCellSize) - (structure.gridGetSizeFull() / 2.f);

			// cellPositionLocal with the transform of the structure applied
			sf::Vector2f cellPositionLocalTransformed = Vector2fMath::rotate(cellPositionLocal, structure.rotation);

			// cell's global position
			cellPosition = structure.position + cellPositionLocalTransformed;
			std::cout << cell.type << std::endl;

			Entity& entity = EntityManager::entityCreateAndGet(instantiationLevel, cell.type);

			auto* entityComponentPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>();

			entityComponentPosition->position = cellPosition;

			auto* entityComponentRotation = entity.entityComponentGet<EntityComponents::ComponentRotation>();
			entityComponentRotation->rotation = structure.rotation + structure.cellGet(x, y).rotation;

		}
	}
}

