#include "GoapGoal.hpp"

void Goap::Goal::preconditionAdd(Precondition precondition) {
	preconditions.insert(precondition);
}
void Goap::Goal::preconditionAdd(BlackboardKey key, Condition condition) {
	preconditionAdd(Precondition(key, condition));
}

void Goap::Goal::insistenceSet(GoalInsistence insistenceNew) {
	insistence = insistenceNew;
}

void Goap::Goal::validationFunctionSet(GoalValidationFunction function) {
	validationFunction = function;
}

bool Goap::Goal::isValid(const Blackboard& blackboard) {
	return std::invoke(validationFunction, blackboard);
}
