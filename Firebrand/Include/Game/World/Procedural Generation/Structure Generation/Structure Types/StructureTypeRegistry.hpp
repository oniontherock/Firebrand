#ifndef __STRUCTURE_TYPE_REGISTRY_H__
#define __STRUCTURE_TYPE_REGISTRY_H__

#include "StructureTypes.hpp"
#include <SFML/System.hpp>

struct StructureTypeHome : StructureTypeBase {
	StructureTypeHome();
};

struct StructureTypeBarn : StructureTypeBase {
	StructureTypeBarn();
};

struct StructureTypeShed : StructureTypeBase {
	StructureTypeShed();
};

#endif