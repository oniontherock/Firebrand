#include "RoomTypeRegistry.hpp"
#include <set>
#include <iostream>

using enum RoomType;
//
//RoomTypeNull::RoomTypeNull() {

//}

std::vector<RoomTypeInstance> RoomTypeRegistry::roomTypeInstances{};

void RoomTypeRegistry::roomInstanceAdd(RoomTypeInstance roomTypeInstance) {
	uint16_t index = static_cast<uint16_t>(roomTypeInstance.type);

	roomTypeInstances.insert(roomTypeInstances.begin() + index, roomTypeInstance);
}
const RoomTypeInstance& RoomTypeRegistry::roomTypeInstanceGet(RoomType roomType) {
	return roomTypeInstances[uint16_t(roomType)];
}
void RoomTypeRegistry::roomTypesRegister() {
	
	roomTypeInstances.resize(uint16_t(RoomType::RoomTypeSize));

	RoomTypeInstance roomTypeInstance = RoomTypeInstance(RoomType::Null);
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Hallway);
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Misc);
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Bedroom);
	roomTypeInstance.constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	roomTypeInstance.constraints.dataSet("RoomSizeRange", std::pair(RoomSize(4, 4), RoomSize(6, 6)));
	roomInstanceAdd(roomTypeInstance);

	//roomTypeInstance = RoomTypeInstance(RoomType::LivingRoom);
	//roomTypeInstance.constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	//roomTypeInstance.constraints.dataSet("RoomSizeRange", std::pair(RoomSize(5, 5), RoomSize(10, 10)));
	//roomInstanceAdd(roomTypeInstance);

	//roomTypeInstance = RoomTypeInstance(RoomType::Laboratory);
	//roomTypeInstance.constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	//roomTypeInstance.constraints.dataSet("RoomSizeRange", std::pair(RoomSize(4, 4), RoomSize(10, 10)));
	//roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Bathroom);
	roomTypeInstance.constraints.dataSet("RoomConnectionsOr", std::set<RoomType>{ Bedroom });
	roomTypeInstance.constraints.dataSet("RoomSizeRange", std::pair(RoomSize(3, 3), RoomSize(6, 6)));
	roomInstanceAdd(roomTypeInstance);
};

