#ifndef __STRUCTURE_INSTANTIATOR_H__
#define __STRUCTURE_INSTANTIATOR_H__

#include "../Grid/StructureGrid.hpp"
#include <World/LevelTypeDefinitions.hpp>

namespace StructureInstantiator {
	void structureInstantiate(LevelPosition instantiationLevel, StructureGrid structure);
}

#endif