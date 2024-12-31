#ifndef __OBJECT_TYPES_H__
#define __OBJECT_TYPES_H__

#include <cstdint>

enum class ObjectType : uint16_t {
	Null, // no object type
	Player,
	SquadMember,
	Door,
	Wall,
	SIZE, // size of the ObjectTypes enum
};



#endif