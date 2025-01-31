#ifndef __STRUCTURE_PLACEMENT_CHANCE_GRID_H__
#define __STRUCTURE_PLACEMENT_CHANCE_GRID_H__

#include "../include/Common/Grid.hpp"
#include <string>
#include <vector>
#include "../../../Structure Generation/Structure Types/StructureTypeRegistry.hpp"
#include "map"

typedef float PlacementChance;

struct StructurePlacementChanceCell {
	std::map<StructureType, PlacementChance> placementChances;
};

typedef std::vector<StructurePlacementChanceCell> StructurePlacementChanceGrid1D;
typedef std::vector<StructurePlacementChanceGrid1D> StructurePlacementChanceGrid2D;

struct StructurePlacementChanceGrid : Grid<StructurePlacementChanceCell> {
	StructurePlacementChanceGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	PlacementChance cellPlacementChanceGet(CellCoordinate cellX, CellCoordinate cellY, StructureType type);
	PlacementChance cellPlacementChanceGet(CellVector cellPos, StructureType type);
	PlacementChance cellPlacementChanceGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, StructureType type);
	PlacementChance cellPlacementChanceGetFromWorld(WorldVector worldPos, StructureType type);

	void cellPlacementChanceSet(CellCoordinate cellX, CellCoordinate cellY, StructureType type, PlacementChance chance);
	void cellPlacementChanceSet(CellVector cellPos, StructureType type, PlacementChance chance);
	void cellPlacementChanceSetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, StructureType type, PlacementChance chance);
	void cellPlacementChanceSetFromWorld(WorldVector worldPos, StructureType type, PlacementChance chance);
};

#endif