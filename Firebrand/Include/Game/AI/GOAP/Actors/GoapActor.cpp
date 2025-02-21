#include "GoapActor.hpp"

#include "../Actions/GoapActionRegistry.hpp"
#include "../Goals/GoapGoalRegistry.hpp"

void Goap::Actor::goalAdd(Goal goal) {
	goals.push_back(goal);
}
void Goap::Actor::goalAdd(GoalName goalName) {
	goalAdd(GoalRegistry::goalGet(goalName));
}

void Goap::Actor::actionAdd(Action action) {
	actions.push_back(action);
}
void Goap::Actor::actionAdd(ActionName actionName) {
	actionAdd(ActionRegistry::actionGet(actionName));
}
