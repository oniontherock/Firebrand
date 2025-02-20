#include "GoapGoal.hpp"

void Goap::Goal::preconditionAdd(Precondition precondition) {
	preconditions.insert(precondition);
}
void Goap::Goal::preconditionAdd(WorldState state, bool preconditionValue) {
	preconditionAdd(Precondition(state, preconditionValue));
}

void Goap::Goal::insistenceSet(GoalInsistence insistenceNew) {
	insistence = insistenceNew;
}

