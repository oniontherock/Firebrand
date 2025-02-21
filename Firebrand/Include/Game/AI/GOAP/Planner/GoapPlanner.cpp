#include "GoapPlanner.hpp"


Goap::GoalName Goap::Planner::actorGoalGet(Actor& actor) {
	return *actor.goals.begin();
}

Goap::Planner::Plan Goap::Planner::actorPlanGet(Actor& actor, Goal& goal) {
	Plan plan;

	PreconditionMap preconditions = goal.preconditions;

	for (Precondition preconditionCur : preconditions) {

		Plan planCur = actionsGetFromPrecondition(preconditionCur, actor.blackboard);
		plan.insert(plan.begin(), planCur.begin(), planCur.end());
	}

	return plan;
}

Goap::Planner::Plan Goap::Planner::planGetFromPrecondition(Precondition precondition, Blackboard blackboard) {
	Plan plan;

	return plan;
}

Goap::Planner::Plan Goap::Planner::actionsGetFromPrecondition(Precondition precondition, Blackboard blackboard) {
	Plan plan;

	// iterate over all possible actions
	for (std::pair<const ActionName, Action>& actionCur : ActionRegistry::actions) {
		
		Action& actionCurInstance = actionCur.second;

		bool actionSatisfies = false;

		// iterate over the effects of each action
		for (std::pair<const BlackboardKey, BlackboardValue>& effectCur : actionCurInstance.effects) {
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
		if (actionCurInstance.preconditions.empty()) {
			if (actionCurInstance.cost < plan.cost) {
				plan.cost = actionCurInstance.cost;
				plan.clear();
				plan.push_back(actionCurInstance); 
			}
		}
		else {
			// initialize a plan that could end up being the new plan if it is better than the current
			Plan possibleNewPlan;

			for (std::pair<const BlackboardKey, Condition>& preconditionCur : actionCurInstance.preconditions) {

				// if the current precondition for the action is satisfied already, go to the next precondition
				if (std::invoke(preconditionCur.second, blackboard.dataUMapGet().at(preconditionCur.first))) continue;

				Plan planCur = actionsGetFromPrecondition(preconditionCur, blackboard);

				possibleNewPlan.insert(possibleNewPlan.end(), planCur.begin(), planCur.end());
			}

			possibleNewPlan.push_back(actionCurInstance);
			possibleNewPlan.cost += actionCurInstance.cost;

			// if the new plan is cheaper than the current, set the current plan to the new plan
			if (possibleNewPlan.cost < plan.cost) {
				plan = possibleNewPlan;
			}
		}
	}

	return plan;
}
