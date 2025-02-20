#include "GoapGoal.hpp"

void Goap::Goal::preconditionAdd(Precondition precondition) {
	preconditions.insert(precondition);
}
void Goap::Goal::preconditionAdd(std::string preconditionName, bool preconditionValue) {
	preconditionAdd(Precondition(preconditionName, preconditionValue));
}

void Goap::Goal::insistenceSet(GoalInsistence insistenceNew) {
	insistence = insistenceNew;
}

