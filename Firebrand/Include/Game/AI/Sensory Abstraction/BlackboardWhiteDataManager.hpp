#ifndef __BLACKBOARD_WHITE_DATA_MANAGER_H__
#define __BLACKBOARD_WHITE_DATA_MANAGER_H__

#include <ECS/Entities/EntityManager.hpp>

namespace Goap {
	// forward declare Blackboard
	struct Blackboard;
}

namespace BlackboardWhiteDataManager {
	void whiteDataObtain(Entity& actor, Goap::Blackboard& blackboard);
}



#endif