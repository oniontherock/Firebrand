#include "GoapActionRegistry.hpp"
#include "../Actors/GoapActor.hpp"

extern std::map<Goap::ActionName, Goap::Action> Goap::ActionRegistry::actions{};

Goap::Action& Goap::ActionRegistry::actionAdd(ActionName actionName) {
	actions.insert({ actionName, Action(actionName) });

	return actions[actionName];
}

void Goap::ActionRegistry::actionsRegister() {

	std::string actionName;

	// we put the action definitions in their own scope so we can have them keep the same name for convience sake
	actionName = "Flee";
	{
		Action& action = actionAdd(actionName);
		action.effectAdd("ThreatDistance", 99999999999999.f); // the action of flee promises to maximum the distance to a threat

		action.executionFunctionSet([](Actor& actor) {
			actor.blackboard.dataSet("ThreatDistance", actor.blackboard.dataGet<float>("ThreatDistance") + 16.f);
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(1.f);
			});
	};
	actionName = "UseMedkit";
	{
		Action& action = actionAdd(actionName);
		action.effectAdd("Health", 99999999999999999.f); // this action basically promises to increase health as much as possible
		action.preconditionAdd("HasMedkit", [](BlackboardValue value) {
			return std::any_cast<bool>(value);
			});

		action.executionFunctionSet([](Actor& actor) {
			actor.blackboard.dataSet("Health", actor.blackboard.dataGet<float>("Health") + 75.f);
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(5.f);
			});
	};
	actionName = "TakeMedkit";
	{
		Action& action = actionAdd(actionName);
		action.effectAdd("HasMedkit", true);
		action.preconditionAdd("MedkitDistance", [](BlackboardValue value) {
			return std::any_cast<float>(value) < 16.f;
			});

		action.executionFunctionSet([](Actor& actor) {
			actor.blackboard.dataSet("HasMedkit", true);
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(3.f);
			});
	};
	actionName = "GoToMedkit";
	{
		Action& action = actionAdd(actionName);
		action.effectAdd("MedkitDistance", 0.f);

		action.executionFunctionSet([](Actor& actor) {
			actor.blackboard.dataSet("MedkitDistance", actor.blackboard.dataGet<float>("MedkitDistance") - 16.f);
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(10.f);
			});
	};
}

Goap::Action Goap::ActionRegistry::actionGet(Goap::ActionName actionName) {
	return actions[actionName];
}

