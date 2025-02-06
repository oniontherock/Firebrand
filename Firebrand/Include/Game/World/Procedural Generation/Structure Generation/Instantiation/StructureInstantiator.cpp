#include "../ACECS/ECSRegistry.hpp"
#include "../ACECS/GameLevel.hpp"
#include "../Floor Generation/FloorGenerator.hpp"
#include "StructureInstantiator.hpp"
#include <bitset>
#include <ECS/Entities/EntityManager.hpp>

void StructureInstantiator::structureInstantiate(LevelPosition instantiationLevel, Structure structure) {
	//GameLevel* level = GameLevelGrid::levelGet(instantiationLevel);

	for (uint16_t x = 0; x < structure.structureGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < structure.structureGrid.gridGetSizeY(); y++) {

			StructureCell cell = structure.structureGrid.cellGet(x, y);

			if (cell.type == StructureCellType("Null")) continue;

			sf::Vector2f cellPosition = sf::Vector2f(0, 0);

			// cell's position local to the structure's origin
			sf::Vector2f cellPositionLocal = (sf::Vector2f(float(x) * structureGridCellSize, float(y) * structureGridCellSize) + cell.offset) - (structure.structureGrid.gridGetSizeFull() / 2.f);

			EntityUpdateType updateType = EntityUpdateType::Frame;

			// if cell.type is a door, set it's update type to frame
			if (cell.type.find("Wall") != std::string::npos) {
				updateType = EntityUpdateType::Never;
			}
			else {
				std::cout << cell.type << "\n";
			}
			//if (cell.type.find("Door") != std::string::npos) {
			//	updateType = EntityUpdateType::Never;
			//}

			//if (updateType == EntityUpdateType::Frame) {
			//	std::cout << cell.type << "\n";
			//}

			// cellPositionLocal with the transform of the structure applied
			sf::Vector2f cellPositionLocalTransformed = Vector2fMath::rotate(cellPositionLocal, structure.rotation);

			// cell's global position
			cellPosition = structure.position + cellPositionLocalTransformed;

			Entity& entity = EntityManager::entityCreateAndGet(instantiationLevel, cell.type, updateType);

			auto* entityComponentPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>();
			entityComponentPosition->position = cellPosition;

			if (entity.entityComponentHas<EntityComponents::ComponentRotation>()) {
				auto* entityComponentRotation = entity.entityComponentGet<EntityComponents::ComponentRotation>();
				entityComponentRotation->rotation = structure.rotation + structure.structureGrid.cellGet(x, y).rotation;
			}
		}
	}

	sf::Vector2f floorGridCellSize = structure.floorGrid.cellsGetSize();

	for (uint16_t x = 0; x < structure.floorGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < structure.floorGrid.gridGetSizeY(); y++) {

			FloorCell cell = structure.floorGrid.cellGet(x, y);

			if (cell == FloorCell("Null")) continue;
			if (cell == FloorCell("")) continue;

			sf::Vector2f cellPosition = sf::Vector2f(0, 0);

			// cell's position local to the structure's origin
			sf::Vector2f cellPositionLocal = (sf::Vector2f(float(x) * floorGridCellSize.x, float(y) * floorGridCellSize.y) - (structure.floorGrid.gridGetSizeFull() / 2.f)) - (floorGridCellSize / 2.f);

			// cellPositionLocal with the transform of the structure applied
			sf::Vector2f cellPositionLocalTransformed = Vector2fMath::rotate(cellPositionLocal, structure.rotation);

			// cell's global position
			cellPosition = structure.position + cellPositionLocalTransformed;

			Entity& entity = EntityManager::entityCreateAndGet(instantiationLevel, "Floor " + cell, EntityUpdateType::Never);

			auto* entityComponentPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>();
			entityComponentPosition->position = cellPosition;

			if (entity.entityComponentHas<EntityComponents::ComponentRotation>()) {
				auto* entityComponentRotation = entity.entityComponentGet<EntityComponents::ComponentRotation>();
				entityComponentRotation->rotation = structure.rotation;
			}
		}
	}
}

