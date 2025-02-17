#include "ObjectTypes.hpp"


extern std::map<ObjectType, std::string> objectTypeToStringMap = {
	{ ObjectType::Null, "Null" },
	{ ObjectType::Human, "Human" },
	{ ObjectType::Door, "Door" },
	{ ObjectType::Dresser, "Dresser" },
	{ ObjectType::Table, "Table" },
	{ ObjectType::Wall, "Wall" },
	{ ObjectType::SIZE, "SIZE" },
};

std::string objectTypeToString(ObjectType objectType) {
	return objectTypeToStringMap[objectType];
}

