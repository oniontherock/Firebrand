#include "GoapActor.hpp"


void Goap::Actor::goalAdd(GoalName goalName) {
	goals.push_back(goalName);
}
void Goap::Actor::actionAdd(ActionName actionName) {
	actions.push_back(actionName);
}
