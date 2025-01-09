#include "RoomTypeRegistry.hpp"
#include <set>

using enum RoomType;

RoomTypeNull::RoomTypeNull() {
	type = RoomType::Null;
}
RoomTypeHallway::RoomTypeHallway() {
	type = RoomType::Hallway;
}
RoomTypeMisc::RoomTypeMisc() {
	type = RoomType::Misc;
}
RoomTypeBedroom::RoomTypeBedroom() {
	type = RoomType::Bedroom;

	constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	constraints.dataSet("RoomSizeRange", std::pair(RoomSize(4, 4), RoomSize(6, 6)));
}
RoomTypeLivingRoom::RoomTypeLivingRoom() {
	type = RoomType::LivingRoom;

	constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	constraints.dataSet("RoomSizeRange", std::pair(RoomSize(5, 5), RoomSize(10, 10)));
}
RoomTypeLaboratory::RoomTypeLaboratory() {
	type = RoomType::Laboratory;

	constraints.dataSet("RoomConnectionsAnd", std::set<RoomType>{ Hallway });
	constraints.dataSet("RoomSizeRange", std::pair(RoomSize(4, 4), RoomSize(10, 10)));
}
RoomTypeBathroom::RoomTypeBathroom() {
	type = RoomType::Bathroom;

	constraints.dataSet("RoomConnectionsOr", std::set<RoomType>{ Hallway, Bedroom, Laboratory, LivingRoom });
	constraints.dataSet("RoomSizeRange", std::pair(RoomSize(3, 3), RoomSize(6, 6)));
}
