#include "GoapActor.hpp"


void Goap::Actor::goalAdd(Goal goal) {
	goals.push_back(goal);
}
void Goap::Actor::actionAdd(Action action) {
	actions.push_back(action);
}
void Goap::Actor::actionsUpdate() {
	for (Action& action : actions) {
		action.actionUpdate(*this);
	}
}
