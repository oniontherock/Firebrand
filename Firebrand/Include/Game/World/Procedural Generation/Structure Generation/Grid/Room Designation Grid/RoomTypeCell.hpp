#ifndef __ROOM_TYPE_CELL_H__
#define __ROOM_TYPE_CELL_H__

#include "../../Room Types/RoomTypes.hpp"
#include <cstdint>

struct RoomTypeCell {

	RoomTypeCell();
	RoomTypeCell(RoomType _type);

	RoomType type;
};


#endif
