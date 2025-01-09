#ifndef __STRUCTURE_TYPE_REGISTRY_H__
#define __STRUCTURE_TYPE_REGISTRY_H__

#include "RoomTypes.hpp"
#include <SFML/System.hpp>

//Null, // walls are set as Null
//Base,
//Hallway,
//Misc, // misc rooms are different from Null, Null isn't even counted as a room, whereas Misc basically counts as a blank room
//Bedroom,
//LivingRoom,
//Laboratory,
//Bathroom,

struct RoomTypeNull : RoomTypeBase {
	RoomTypeNull();
};
struct RoomTypeHallway: RoomTypeBase {
	RoomTypeHallway();
};
struct RoomTypeMisc : RoomTypeBase {
	RoomTypeMisc();
};
struct RoomTypeBedroom : RoomTypeBase {
	RoomTypeBedroom();
};
struct RoomTypeLivingRoom : RoomTypeBase {
	RoomTypeLivingRoom();
};
struct RoomTypeLaboratory : RoomTypeBase {
	RoomTypeLaboratory();
};
struct RoomTypeBathroom : RoomTypeBase {
	RoomTypeBathroom();
};

#endif