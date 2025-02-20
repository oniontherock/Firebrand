#ifndef __GOAP_GOAL_H__
#define __GOAP_GOAL_H__

#include "../GoapTypes.hpp"
#include <string>

namespace Goap {

	typedef std::string GoalName;
	typedef float GoalInsistence;

	struct Goal {

		PreconditionMap preconditions;

		GoalInsistence insistence = 0.5f;

		void preconditionAdd(Precondition precondition);
		void preconditionAdd(WorldState state, bool preconditionValue);
		void insistenceSet(GoalInsistence insistenceNew);


	};
}

#endif
 