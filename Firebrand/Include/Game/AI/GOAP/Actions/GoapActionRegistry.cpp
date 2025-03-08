#include "GoapActionRegistry.hpp"
#include "../Actors/GoapActor.hpp"
#include "../ACECS/ECSRegistry.hpp"

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
		action.effectAdd("ThreatCount", uint32_t(0));

		action.executionFunctionSet([](Entity& entity, Actor& actor) {

			//float angleToEnemyClosest = actor.blackboard.ThreatClosestPolarCoordinates.y;

			//entity.entityEventAddAndGet<EntityEvents::EventMoveDirection>()->moveDirection = -sf::Vector2f(cos(angleToEnemyClosest), sin(angleToEnemyClosest));
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(1.f);
			});
	};
}

Goap::Action Goap::ActionRegistry::actionGet(Goap::ActionName actionName) {
	return actions[actionName];
}

