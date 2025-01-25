#include "PropTypeRegistry.hpp"
#include <Auxiliary/Math.hpp>
#include <iostream>
#include <set>

using enum PropType;

std::vector<PropTypeInstance> PropTypeRegistry::propTypeInstances{};

void PropTypeRegistry::propInstanceAdd(PropTypeInstance propTypeInstance) {
	uint16_t index = static_cast<uint16_t>(propTypeInstance.type);

	propTypeInstances.insert(propTypeInstances.begin() + index, propTypeInstance);
}
const PropTypeInstance& PropTypeRegistry::propTypeInstanceGet(PropType roomType) {
	return propTypeInstances[uint16_t(roomType)];
}
void PropTypeRegistry::propTypesRegister() {

	propTypeInstances.resize(uint16_t(PropType::PropTypeSize));

	PropTypeInstance roomTypeInstance = PropTypeInstance(PropType::Null);
	propInstanceAdd(roomTypeInstance);

	roomTypeInstance = PropTypeInstance(PropType::Dresser);
	roomTypeInstance.name = "Dresser";
	roomTypeInstance.constraints.dataSet("IsOffCenter", NULL);
	roomTypeInstance.constraints.dataSet("IsWallAdjacent", NULL);
	roomTypeInstance.constraints.dataSet("IsFlushWithWall", NULL); // rotates the prop away from the nearest wall and moves the prop so that it is flush with the wall, requires the object to be adjacent to a wall to take affect
	roomTypeInstance.distToBeWallFlush = 48.f;
	propInstanceAdd(roomTypeInstance);

	roomTypeInstance = PropTypeInstance(PropType::Table);
	roomTypeInstance.name = "Table";
	roomTypeInstance.constraints.dataSet("IsOffEdge", NULL);
	propInstanceAdd(roomTypeInstance);
};

