#ifndef __ROOM_TYPES_H__
#define __ROOM_TYPES_H__

#include <any>
#include <cstdint>
#include <vector>
#include <SFML/System.hpp>
#include "../include/Common/DataCache.hpp"
#include "../Prop Types/PropTypes.hpp"

enum class RoomType {
	// extras
	Null, // walls are set as Null
	Hallway,
	Misc, // misc rooms are different from Null, Null isn't even counted as a room, whereas Misc basically counts as a blank room
	// residential
	Bedroom,
	///Closet,
	///FamilyRoom,
	LivingRoom,
	///Kitchen,
	///LaundryRoom,
	// lab/research
	//Laboratory,
	// common
	Bathroom,
	// size of the RoomType enum
	RoomTypeSize,
};

using RoomSize = sf::Vector2u;

typedef DataCache RoomConstraints;

struct RoomTypeInstance {
	RoomTypeInstance() = default;
	RoomTypeInstance(RoomType _type);

	// the type of room this is
	RoomType type = RoomType::Null;
	// constraints for this room
	RoomConstraints constraints;
	// the type floor that is used in this room type
	std::string floorType = "";
	// prop types allowed in this room
	std::vector<PropType> propTypes;
};

#endif