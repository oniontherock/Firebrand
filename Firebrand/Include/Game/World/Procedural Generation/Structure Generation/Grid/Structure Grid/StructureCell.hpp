#ifndef __STRUCTURE_CELL_H__
#define __STRUCTURE_CELL_H__

#include <string>

// type of a StructureCell, used for checking what kind of cell a cell is, and for instancing entities from templates
typedef std::string StructureCellType;

struct StructureCell {

	StructureCell();
	StructureCell(StructureCellType _type, float _rotation);

	StructureCellType type;

	float rotation;
};


#endif
