#ifndef __PROP_TYPE_REGISTRY_H__
#define __PROP_TYPE_REGISTRY_H__

#include "PropTypes.hpp"
#include <cstdint>
#include <SFML/System.hpp>

class PropTypeRegistry {

	static std::vector<PropTypeInstance> propTypeInstances;

	static void propInstanceAdd(PropTypeInstance propTypeInstance);

	template <typename T>
	static void propConstraintAdd(PropType roomType, DataKey key, T value) {
		propTypeInstances[static_cast<uint16_t>(roomType)].constraints.dataSet(key, value);
	}

public:

	static const PropTypeInstance& propTypeInstanceGet(PropType roomType);

	static void propTypesRegister();
};

#endif