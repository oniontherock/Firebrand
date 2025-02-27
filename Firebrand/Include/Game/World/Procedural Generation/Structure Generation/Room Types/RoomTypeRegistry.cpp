#include "RoomTypeRegistry.hpp"
#include <set>
#include <iostream>

using enum RoomType;

std::vector<RoomTypeInstance> RoomTypeRegistry::roomTypeInstances{};

void RoomTypeRegistry::roomInstanceAdd(RoomTypeInstance roomTypeInstance) {
	uint16_t index = static_cast<uint16_t>(roomTypeInstance.type);

	roomTypeInstances.insert(roomTypeInstances.begin() + index, roomTypeInstance);
}
RoomTypeInstance& RoomTypeRegistry::roomTypeInstanceGet(RoomType roomType) {
	return roomTypeInstances[uint16_t(roomType)];
}
void RoomTypeRegistry::roomTypesRegister() {
	
	roomTypeInstances.resize(uint16_t(RoomType::RoomTypeSize));

	RoomTypeInstance roomTypeInstance = RoomTypeInstance(RoomType::Null);
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Hallway);
	roomTypeInstance.floorType = "Plank";
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Misc);
	roomTypeInstance.floorType = "Plank";
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Bedroom);
	roomTypeInstance.constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	roomTypeInstance.constraints.dataSet("RoomDimensionsRange", std::pair(RoomSize(4, 4), RoomSize(12, 10)));
	roomTypeInstance.constraints.dataSet<uint16_t>("RoomTypeCount", 6u);
	roomTypeInstance.floorType = "Plank";
	roomTypeInstance.propTypes = { PropType::Dresser };
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::LivingRoom);
	roomTypeInstance.constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	roomTypeInstance.constraints.dataSet("RoomSizeRange", std::pair<uint16_t, uint16_t>(uint16_t(6 * 6), uint16_t(32 * 32)));
	roomTypeInstance.constraints.dataSet<uint16_t>("RoomTypeCount", 2u);
	roomTypeInstance.floorType = "Plank";
	roomTypeInstance.propTypes = { PropType::Table };
	roomInstanceAdd(roomTypeInstance);

	roomTypeInstance = RoomTypeInstance(RoomType::Bathroom);
	roomTypeInstance.constraints.dataSet("RoomConnectionsOr", std::set<RoomType>{ Bedroom, Hallway, LivingRoom });
	roomTypeInstance.constraints.dataSet("RoomConnectionsNand", std::set<RoomType>{ Bathroom });
	roomTypeInstance.constraints.dataSet("RoomSizeRange", std::pair<uint16_t, uint16_t>(uint16_t(3 * 4), uint16_t(6 * 6)));
	roomTypeInstance.constraints.dataSet<uint16_t>("RoomTypeCount", 3u);
	roomTypeInstance.floorType = "Tile";
	roomInstanceAdd(roomTypeInstance);
};

