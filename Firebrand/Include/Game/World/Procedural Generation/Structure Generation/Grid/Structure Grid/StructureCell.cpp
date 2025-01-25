#include "StructureCell.hpp"

StructureCell::StructureCell() :
	type("Null"),
	rotation(0.f)
{
}
StructureCell::StructureCell(StructureCellType _type, float _rotation, sf::Vector2f _offset) :
	type(_type),
	rotation(_rotation),
	offset(_offset)
{
}
StructureCell::StructureCell(StructureCellType _type, float _rotation) :
	StructureCell(_type, _rotation, sf::Vector2f(0, 0))
{
}
