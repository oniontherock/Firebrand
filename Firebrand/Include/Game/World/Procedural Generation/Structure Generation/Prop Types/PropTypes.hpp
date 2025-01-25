#ifndef __PROP_TYPES_H__
#define __PROP_TYPES_H__

#include <any>
#include <cstdint>
#include <vector>
#include <SFML/System.hpp>
#include "../include/Common/DataCache.hpp"
#include <string>


enum class PropType {
	Null,
	Dresser,
	Table,
	// size of the PropType enum
	PropTypeSize,
};

typedef std::vector<std::string> PropTypeGrid1D;
typedef std::vector<PropTypeGrid1D> PropTypeGrid2D;

typedef DataCache PropConstraints;

struct PropTypeInstance {
	PropTypeInstance() = default;
	PropTypeInstance(PropType _type);

	// the type of prop this is
	PropType type = PropType::Null;
	// constraints for this prop
	PropConstraints constraints;
	// the name of the template that this prop corresponds to 
	std::string name;
};
#endif