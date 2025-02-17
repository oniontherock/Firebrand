#ifndef __OBJECT_ABSTRACTOR_H__
#define __OBJECT_ABSTRACTOR_H__

#include "../ACECS/ECSRegistry.hpp"
#include "../include/Common/DataCache.hpp"
#include "../include/Game/World/Objects/ObjectRegistry.hpp"
#include "../include/Game/World/Objects/ObjectTypes.hpp"

namespace ObjectAbstractor {
	DataCache objectDataAbstract(EntityId objectId, ObjectType objectType);
}

#endif