#ifndef __GOAP_ACTOR_H__
#define __GOAP_ACTOR_H__

#include "../Actions/GoapAction.hpp"
#include "../Goals/GoapGoal.hpp"


namespace Goap {
	struct Actor {

		void goalAdd(GoalName goalName);
		void actionAdd(ActionName actionName);

		std::vector<ActionName> actions;
		std::vector<GoalName> goals;

		Blackboard blackboard;
	};
}

#endif