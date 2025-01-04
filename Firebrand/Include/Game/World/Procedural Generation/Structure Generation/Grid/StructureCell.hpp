#ifndef __STRUCTURE_CELL_H__
#define __STRUCTURE_CELL_H__


struct StructureCell {

	StructureCell();
	StructureCell(const char* _type, float _rotation);

	const char* type;

	float rotation;
};


#endif
