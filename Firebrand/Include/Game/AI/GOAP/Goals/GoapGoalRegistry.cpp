#include "GoapGoalRegistry.hpp"


extern std::map<Goap::GoalName, Goap::Goal> Goap::GoalRegistry::goals{};

Goap::Goal& Goap::GoalRegistry::goalAdd(GoalName goalName) {
	goals.insert({ goalName, Goal() });

	return goals[goalName];
}

void Goap::GoalRegistry::goalsRegister() {

	// we put the goal definitions in their own scope so we can have them keep the same name for convience sake
	{
		Goal& goal = goalAdd("KeepSafe");
		goal.insistenceSet(0.8f);
		goal.preconditionAdd("ThreatDistance", [](BlackboardValue value) {
			return std::any_cast<float>(value) > 1024.f;
			}
		);
		goal.preconditionAdd("Health", [](BlackboardValue value) {
			return std::any_cast<float>(value) > 75.f;
			}
		);
		//goal.preconditionAdd("HasWeapon", [](BlackboardValue value) {
		//	return std::any_cast<bool>(value);
		//	}
		//);
		goal.validationFunctionSet([](const Blackboard& blackboard) {
			return true;
			});
	};
}
Goap::Goal Goap::GoalRegistry::goalGet(GoalName goalName) {
	return goals[goalName];
}

