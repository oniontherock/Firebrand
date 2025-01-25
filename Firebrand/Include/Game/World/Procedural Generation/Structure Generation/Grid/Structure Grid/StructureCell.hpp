#ifndef __STRUCTURE_CELL_H__
#define __STRUCTURE_CELL_H__

#include <string>
#include <SFML/System/Vector2.hpp>

// type of a StructureCell, used for checking what kind of cell a cell is, and for instancing entities from templates
typedef std::string StructureCellType;

struct StructureCell {

	StructureCell();
	StructureCell(StructureCellType _type, float _rotation, sf::Vector2f _offset);
	StructureCell(StructureCellType _type, float _rotation);

	StructureCellType type;

	float rotation;

	sf::Vector2f offset;
};


#endif
