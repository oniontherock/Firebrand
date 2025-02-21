#ifndef __GOAP_ACTOR_H__
#define __GOAP_ACTOR_H__

#include "../Actions/GoapAction.hpp"
#include "../Goals/GoapGoal.hpp"


namespace Goap {
	struct Actor {

		void goalAdd(Goal goal);
		void actionAdd(Action action);

		std::vector<Action> actions;
		std::vector<Goal> goals;

		Blackboard blackboard;

		void actionsUpdate();
	};
}

#endif