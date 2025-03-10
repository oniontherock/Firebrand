#ifndef __GOAP_ACTOR_H__
#define __GOAP_ACTOR_H__

#include "../Actions/GoapAction.hpp"
#include "../Goals/GoapGoal.hpp"


namespace Goap {
	struct Actor {

		void goalAdd(Goal goal);
		void goalAdd(GoalName goalName);

		void actionAdd(Action action);
		void actionAdd(ActionName actionName);

		std::vector<Action> actions;
		std::vector<Goal> goals;

		Blackboard blackboard;
	};
}

#endif