#include "GoapGoalRegistry.hpp"


extern std::map<Goap::GoalName, Goap::Goal> Goap::GoalRegistry::goals{};

Goap::Goal& Goap::GoalRegistry::goalAdd(GoalName goalName) {
	goals.insert({ goalName, Goal(goalName) });

	return goals[goalName];
}

void Goap::GoalRegistry::goalsRegister() {

	// we put the goal definitions in their own scope so we can have them keep the same name for convience sake
	{
		Goal& goal = goalAdd("KeepSafe");
		goal.insistenceSet(0.8f);
		goal.preconditionAdd("ThreatCount", [](BlackboardValue value) {
			return std::any_cast<uint32_t>(value) <= 0;
			}
		);
		goal.validationFunctionSet([](const Blackboard&) {
			return true;
			});
	};
}
Goap::Goal Goap::GoalRegistry::goalGet(GoalName goalName) {
	return goals[goalName];
}

