#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#include "Grid/Floor Grid/FloorGrid.hpp"
#include "Grid/Structure Grid/StructureGrid.hpp"

class Structure {
public:
	Structure(StructureGrid _structureGrid, FloorGrid _floorGrid, StructureType _structureType, sf::Vector2f _position, float _rotation);

	// the StructureGrid for this structure
	StructureGrid structureGrid = StructureGrid(0, 0);
	// the FloorGrid for this structure
	FloorGrid floorGrid = FloorGrid(0, 0);
	// type of structure this is
	StructureType structureType;
	// structure's position in the world
	sf::Vector2f position;
	// structure's global rotation
	float rotation;
};


#endif
