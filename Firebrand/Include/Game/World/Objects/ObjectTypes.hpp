#ifndef __OBJECT_TYPES_H__
#define __OBJECT_TYPES_H__

#include <cstdint>
#include <map>
#include <string>

enum class ObjectType : uint16_t {
	Null, // no object type
	Human,
	Door,
	Dresser,
	Table,
	Wall,
	SIZE, // size of the ObjectTypes enum
};

extern std::map<ObjectType, std::string> objectTypeToStringMap;

std::string objectTypeToString(ObjectType objectType);

#endif