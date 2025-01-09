#include "StructureTypes.hpp"
#include <Auxiliary/NumberGenerator.hpp>

RoomSize StructureTypeBase::roomSizeInstanceGet() const {

	// get room dimensions in the min/max range.
	// we do plus one because getRange returns a value in the range of A to B-1, and in this case we just want A to B.
	uint16_t sizeX = RNGu16::getRange(roomSizeMin.x, roomSizeMax.x + 1);
	uint16_t sizeY = RNGu16::getRange(roomSizeMin.y, roomSizeMax.y + 1);

	return sf::Vector2u(sizeX, sizeY);
}
RoomCount StructureTypeBase::roomCountInstanceGet() const {
	// get room count in the min/max range.
	// we do plus one because getRange returns a value in the range of A to B-1, and in this case we just want A to B.
	return RNGu16::getRange(roomCountMin, roomCountMax + 1);
}
RoomContactCount StructureTypeBase::roomContactCountInstanceGet() const {
	// get room contact count in the min/max range.
	// we do plus one because getRange returns a value in the range of A to B-1, and in this case we just want A to B.
	return RNGu16::getRange(roomContactCountMin, roomContactCountMax + 1);
}