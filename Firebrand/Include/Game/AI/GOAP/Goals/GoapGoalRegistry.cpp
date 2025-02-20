#include "GoapGoalRegistry.hpp"


extern std::map<Goap::GoalName, Goap::Goal> Goap::goals{};

Goap::Goal& Goap::goalAdd(GoalName goalName) {
	goals.insert({ goalName, Goal() });

	return goals[goalName];
}

void Goap::goalsRegister() {
	// we put the goal definitions in their own scope so we can have them keep the same name for convience sake
	{
		Goal& goal = goalAdd("KeepSafe");
		goal.insistenceSet(0.8f);
		goal.preconditionAdd("IsThreatNear", false);
		goal.preconditionAdd("IsHealthLow", false);
		goal.preconditionAdd("HasWeapon", true);
	};
	{
		Goal& goal = goalAdd("KeepFed");
		goal.insistenceSet(0.2f);
		goal.preconditionAdd("IsHungry", false);
		goal.preconditionAdd("HasFood", true);
	};
	{
		Goal& goal = goalAdd("WithTeam");
		goal.insistenceSet(0.4f);
		goal.preconditionAdd("IsAllyNear", true);
		goal.preconditionAdd("IsLeaderNear", true);
	};
}
