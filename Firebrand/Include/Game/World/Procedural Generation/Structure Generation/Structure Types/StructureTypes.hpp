#ifndef __STRUCTURE_TYPES_H__
#define __STRUCTURE_TYPES_H__

#include <cstdint>
#include <SFML/System.hpp>

enum class StructureType : uint16_t {
	Base, // base type of StructureType struct
	Home,
	Barn,
	Shed,
};

using RoomSize = sf::Vector2u;
typedef uint16_t RoomCount;
typedef uint16_t RoomContactCount;

struct StructureTypeBase {
	StructureTypeBase() = default;

	// the type of structure this is
	StructureType structureType = StructureType::Base;

	// minimum possible size for rooms in this structure
	RoomSize roomSizeMin = RoomSize(0, 0);
	// maximum possible size for rooms in this structure
	RoomSize roomSizeMax = RoomSize(0, 0);

	// minimum possible amount of rooms in this structure
	RoomCount roomCountMin = 0;
	// maximum possible amount of rooms in this structure
	RoomCount roomCountMax = 0;

	// minimum possible amount of contacts that rooms in this structure require
	RoomContactCount roomContactCountMin = 0;
	// maximum possible amount of contacts that rooms in this structure require
	RoomContactCount roomContactCountMax = 0;

	// whether rooms in this structure need full contact or partial contact for something to contact as a contact.
	// basically, true means that an entire face of a room must have contact with another for it to count as a contact,
	// false means that any contact on one of the room's faces count.
	// generally, true causes more sporadic, crazy buildings, and false makes more predictable buildings.
	bool fullContact = true;

	// gets a RoomSize in this structure's room size min and max range
	RoomSize roomSizeInstanceGet() const;
	// gets a RoomCount in this structure's room count min and max range
	RoomCount roomCountInstanceGet() const;
	// gets a RoomContactCount in this structure's room contact count min and max range
	RoomContactCount roomContactCountInstanceGet() const;
};

#endif