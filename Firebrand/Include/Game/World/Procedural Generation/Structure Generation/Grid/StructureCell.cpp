#include "StructureCell.hpp"

StructureCell::StructureCell() :
	type("Null"),
	rotation(0.f)
{
}

StructureCell::StructureCell(StructureCellType _type, float _rotation) :
	type(_type),
	rotation(_rotation)
{
}
