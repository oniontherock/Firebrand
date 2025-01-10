#ifndef __STRUCTURE_TYPE_REGISTRY_H__
#define __ROOM_TYPE_REGISTRY_H__

#include "RoomTypes.hpp"
#include <cstdint>
#include <SFML/System.hpp>

class RoomTypeRegistry {

	static std::vector<RoomTypeInstance> roomTypeInstances;

	static void roomInstanceAdd(RoomTypeInstance roomTypeInstance);

	template <typename T>
	static void roomConstraintAdd(RoomType roomType, DataKey key, T value) {
		roomTypeInstances[static_cast<uint16_t>(roomType)].constraints.dataSet(key, value);
	}

public:

	static const RoomTypeInstance& roomTypeInstanceGet(RoomType roomType);

	static void roomTypesRegister();
};

#endif