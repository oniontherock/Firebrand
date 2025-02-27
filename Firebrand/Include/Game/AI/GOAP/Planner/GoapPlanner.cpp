#include "GoapPlanner.hpp"
#include <iostream>


Goap::Goal Goap::Planner::actorGoalGet(Actor& actor) {
	return *actor.goals.begin();
}

Goap::Planner::Plan Goap::Planner::actorPlanGet(Actor& actor, Goal& goal) {
	Plan plan;
	plan.cost = 0.f;

	PreconditionMap preconditions = goal.preconditions;

	for (Precondition preconditionCur : preconditions) {
		if (std::invoke(preconditionCur.second, actor.blackboard.dataUMapGet().at(preconditionCur.first))) continue;

		Plan planCur = actionsGetFromPrecondition(preconditionCur, actor, actor.blackboard);
		plan.insert(plan.begin(), planCur.begin(), planCur.end());
		plan.cost += planCur.cost;
	}

	return plan;
}

Goap::Planner::Plan Goap::Planner::actionsGetFromPrecondition(Precondition precondition, Actor& actor, Blackboard blackboard) {
	Plan plan;

	// iterate over all possible actions
	for (Action& actionCur : actor.actions) {
		
		actionCur.evaluate(actor);

		bool actionSatisfies = false;

		// iterate over the effects of each action
		for (std::pair<const BlackboardKey, BlackboardValue>& effectCur : actionCur.effects) {
			// skip if effect and precondition are not the same blackboard key (I.E. they would not be comparing the same variable)
			if (effectCur.first != precondition.first) continue;

			// if this effect satisfies the precondition, mark that the action satisfies the precondition, and stop checking effects
			if (std::invoke(precondition.second, effectCur.second)) {
				actionSatisfies = true;
				break;
			}
		}

		if (!actionSatisfies) continue;

		// if there are no preconditions for the action, then we just check if the action is cheaper than whatever plan we currently have, and if it is, that single action becomes the new plan
		if (actionCur.preconditions.empty()) {
			if (actionCur.cost < plan.cost) {
				plan.cost = actionCur.cost;
				plan.clear();
				plan.push_back(actionCur);
			}
		}
		else {
			// initialize a plan that could end up being the new plan if it is better than the current
			Plan possibleNewPlan;
			possibleNewPlan.cost = 0.f;

			for (std::pair<const BlackboardKey, Condition>& preconditionCur : actionCur.preconditions) {

				// if the current precondition for the action is satisfied already, go to the next precondition
				if (std::invoke(preconditionCur.second, blackboard.dataUMapGet().at(preconditionCur.first))) continue;

				Plan planCur = actionsGetFromPrecondition(preconditionCur, actor, blackboard);

				possibleNewPlan.insert(possibleNewPlan.end(), planCur.begin(), planCur.end());
				possibleNewPlan.cost += planCur.cost;
			}

			possibleNewPlan.push_back(actionCur);
			possibleNewPlan.cost += actionCur.cost;

			// if the new plan is cheaper than the current, set the current plan to the new plan
			if (possibleNewPlan.cost < plan.cost) {
				plan = possibleNewPlan;
			}
		}
	}

	return plan;
}
