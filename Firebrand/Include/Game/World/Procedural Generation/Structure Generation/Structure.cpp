#include "Structure.hpp"

Structure::Structure(StructureGrid _structureGrid, FloorGrid _floorGrid, StructureType _structureType, sf::Vector2f _position, float _rotation) {

	structureGrid = _structureGrid;
	floorGrid = _floorGrid;
	structureType = _structureType;
	position = _position;
	rotation = _rotation;
}
