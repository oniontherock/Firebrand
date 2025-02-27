#ifndef __GOAP_GOAL_H__
#define __GOAP_GOAL_H__

#include "../Blackboard/GoapBlackboard.hpp"
#include "../GoapTypes.hpp"
#include <string>
#include <functional>

namespace Goap {

	typedef std::string GoalName;
	typedef float GoalInsistence;

	typedef std::function<bool(const Blackboard&)> GoalValidationFunction;

	struct Goal {

		Goal();
		Goal(GoalName _name);

		// primarily use for debugging so when can tell what goals are being used
		GoalName name;

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
 