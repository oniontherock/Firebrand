#include "../../Sensory Abstraction/ActorDataHelper.hpp"
#include "../ACECS/ECSRegistry.hpp"
#include "../Actors/GoapActor.hpp"
#include "GoapActionRegistry.hpp"

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
		action.effectAdd("ThreatClosestPolarCoordinates", sf::Vector2f(9999999999.f, 0.f));

		action.executionFunctionSet([](Entity& entity, Actor& actor) {

			sf::Vector2f threatClosestPolarCoordinates = actor.blackboard.whiteDataGet<sf::Vector2f>("ThreatClosestPolarCoordinates");

			sf::Vector2f entityPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>()->position;

			sf::Vector2f fleePosition = entityPosition;
			while (threatClosestPolarCoordinates.x < 512.f) {

				fleePosition += sf::Vector2f(cos(threatClosestPolarCoordinates.y), sin(threatClosestPolarCoordinates.y)) * -256.f;

				threatClosestPolarCoordinates = ActorDataHelper::pointGetClosestThreatPolarCoordinates(actor, fleePosition);
				std::cout << threatClosestPolarCoordinates.x << "\n";
			}

			entity.entityEventAddAndGet<EntityEvents::EventMovementTargetSet>()->target = fleePosition;
			entity.entityEventAddAndGet<EntityEvents::EventMovementStateSet>()->movementState = Movement::MovementState::Run;
			});
		action.evaluationFunctionSet([](Actor&) {
			return ActionCost(1.f);
			});
	};
}

Goap::Action Goap::ActionRegistry::actionGet(Goap::ActionName actionName) {
	return actions[actionName];
}

