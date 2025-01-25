#include "PropTypeRegistry.hpp"
#include <set>
#include <iostream>

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
	propInstanceAdd(roomTypeInstance);

	roomTypeInstance = PropTypeInstance(PropType::Table);
	roomTypeInstance.name = "Table";
	propInstanceAdd(roomTypeInstance);
};

