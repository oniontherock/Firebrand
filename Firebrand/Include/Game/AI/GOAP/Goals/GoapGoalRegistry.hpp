#ifndef __GOAP_GOAL_REGISTRY_H__
#define __GOAP_GOAL_REGISTRY_H__

#include "GoapGoal.hpp"
#include <map>
#include <string>


namespace Goap {
	namespace GoalRegistry {
		extern std::map<GoalName, Goal> goals;

		Goal& goalAdd(GoalName goalName);

		void goalsRegister();
	}
}

#endif
