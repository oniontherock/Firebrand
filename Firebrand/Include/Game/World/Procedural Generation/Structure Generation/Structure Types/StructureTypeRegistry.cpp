#include "StructureTypeRegistry.hpp"

StructureTypeHome::StructureTypeHome() {
	// structure type
	structureType = StructureType::Home;
	// room size
	roomSizeMin = RoomSize(5, 5), roomSizeMax = RoomSize(8, 8);
	// room count
	roomCountMin = 16, roomCountMax = 24;
	// room contact count
	roomContactCountMin = 2, roomContactCountMax = 4;
	// full contact
	fullContact = true;
}

StructureTypeBarn::StructureTypeBarn() {
	// structure type
	structureType = StructureType::Barn;
	// room size
	roomSizeMin = RoomSize(UINT16_MAX, 3), roomSizeMax = RoomSize(UINT16_MAX, 5);
	// room count
	roomCountMin = 0, roomCountMax = 2;
	// room contact count
	roomContactCountMin = 3, roomContactCountMax = 3;
	// full contact
	fullContact = true;
}
StructureTypeShed::StructureTypeShed() {
	// structure type
	structureType = StructureType::Shed;
	// room size
	roomSizeMin = RoomSize(0, 0), roomSizeMax = RoomSize(0, 0);
	// room count
	roomCountMin = 0, roomCountMax = 0;
	// room contact count
	roomContactCountMin = 0, roomContactCountMax = 0;
	// full contact
	fullContact = true;
}

