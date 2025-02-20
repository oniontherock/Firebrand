#ifndef __GOAP_GOAL_H__
#define __GOAP_GOAL_H__

#include "../GoapTypes.hpp"
#include <string>

namespace Goap {

	typedef std::string GoalName;
	typedef float GoalInsistence;

	typedef std::function<bool(const Blackboard& blackboard)> GoalValidationFunction;

	struct Goal {

		PreconditionMap preconditions;

		GoalInsistence insistence = 0.5f;

		void preconditionAdd(Precondition precondition);
		void preconditionAdd(BlackboardKey key, Condition condition);
		void insistenceSet(GoalInsistence insistenceNew);

		void validationFunctionSet(GoalValidationFunction function);
		bool isValid(const Blackboard& blackboard);

	private:
		GoalValidationFunction validationFunction = [](const Blackboard&) { return true; };
	};
}

#endif
 