#include "GoapActionRegistry.hpp"

extern std::map<Goap::ActionName, Goap::Action> Goap::actions{};

Goap::Action& Goap::actionAdd(ActionName actionName) {
	actions.insert({ actionName, Action() });

	return actions[actionName];
}

void Goap::actionsRegister() {

	using enum WorldState;

	std::string actionName;

	// we put the action definitions in their own scope so we can have them keep the same name for convience sake
	actionName = "Flee";
	{
		Action& action = actionAdd(actionName);
		action.effectAdd(IsThreatNear, false);

		action.executionFunctionSet([](Actor&) {
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(25.f);
			});
	};
	actionName = "EnterBuilding";
	{
		Action& action = actionAdd(actionName);
		action.effectAdd(IsInBuilding, true);

		action.executionFunctionSet([](Actor&) {
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(5.f);
			});
	};
}
