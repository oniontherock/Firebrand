#ifndef __GOAP_PLANNER_H__
#define __GOAP_PLANNER_H__

#include "../Actions/GoapAction.hpp"
#include "../Actions/GoapActionRegistry.hpp"
#include "../Actors/GoapActor.hpp"
#include "../Blackboard/GoapBlackboard.hpp"
#include "../Goals/GoapGoal.hpp"
#include "../Goals/GoapGoalRegistry.hpp"
#include "../GoapTypes.hpp"

namespace Goap {
	namespace Planner {

		struct Plan : std::vector<Action> {
			float cost = 99999999999.f;
		};

		GoalName actorGoalGet(Actor& actor);
		Plan actorPlanGet(Actor& actor, Goal& goal);

		Plan planGetFromPrecondition(Precondition precondition, Blackboard blackboard);
		// returns a vector of actions that satisfy the precondition
		Plan actionsGetFromPrecondition(Precondition precondition, Blackboard blackboard);
	}
}


#endif
