#include "GoapAction.hpp"

Goap::Action::Action() {
	name = "NULL NAME";
}
Goap::Action::Action(ActionName _name) {
	name = _name;
}

void Goap::Action::preconditionAdd(Precondition precondition) {
	preconditions.insert(precondition);
}
void Goap::Action::preconditionAdd(BlackboardKey key, Condition condition) {
	preconditionAdd(Precondition(key, condition));
}

void Goap::Action::effectAdd(Effect effect) {
	effects.insert(effect);
}
void Goap::Action::effectAdd(BlackboardKey key, BlackboardValue value) {
	effectAdd(Effect(key, value));
}

void Goap::Action::costSet(ActionCost costNew) {
	cost = costNew;
}

void Goap::Action::executionFunctionSet(ActionExecutionFunction function) {
	executionFunction = function;
}
void Goap::Action::evaluationFunctionSet(ActionEvaluationFunction function) {
	evaluationFunction = function;
}

void Goap::Action::execute(Entity& entity, Actor& actor) {
	std::invoke(executionFunction, entity, actor);
}
void Goap::Action::evaluate(Actor& actor) {
	costSet(std::invoke(evaluationFunction, actor));
}