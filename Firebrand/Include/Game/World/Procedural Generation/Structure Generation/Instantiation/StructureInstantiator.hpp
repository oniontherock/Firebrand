#ifndef __STRUCTURE_INSTANTIATOR_H__
#define __STRUCTURE_INSTANTIATOR_H__

#include "../Grid/Structure Grid/StructureGrid.hpp"
#include "../Structure.hpp"
#include <World/LevelTypeDefinitions.hpp>

namespace StructureInstantiator {
	void structureInstantiate(LevelPosition instantiationLevel, Structure structure);
}

#endif