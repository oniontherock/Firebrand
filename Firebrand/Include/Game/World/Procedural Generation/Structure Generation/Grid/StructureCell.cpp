#include "StructureCell.hpp"

StructureCell::StructureCell() :
	type("Null")
{
}

StructureCell::StructureCell(const char* _type, float _rotation) :
	type(_type),
	rotation(_rotation)
{
}
