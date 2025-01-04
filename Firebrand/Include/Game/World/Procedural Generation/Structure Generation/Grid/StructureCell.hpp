#ifndef __STRUCTURE_CELL_H__
#define __STRUCTURE_CELL_H__

// type of a StructureCell, used for checking what kind of cell a cell is, and for instancing entities from templates
typedef const char* StructureCellType;

struct StructureCell {

	StructureCell();
	StructureCell(StructureCellType _type, float _rotation);

	StructureCellType type;

	float rotation;
};


#endif
