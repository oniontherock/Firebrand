#ifndef __ROOM_TYPE_H__
#define __ROOM_TYPE_H__

enum class RoomType {
	// misc
	Null, // walls are set as Null
	Hallway,
	Misc, // misc rooms are different from Null, Null isn't even counted as a room, whereas Misc basically counts as a blank room
	// residential
	Bedroom,
	//Closet,
	//FamilyRoom,
	LivingRoom,
	//Kitchen,
	//LaundryRoom,
	// lab/research
	Laboratory,
	// common
	Bathroom,
	// size of the RoomType enum
	RoomTypeSize,
};

#endif