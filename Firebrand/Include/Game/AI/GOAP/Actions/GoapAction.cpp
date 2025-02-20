#include "GoapAction.hpp"

void Goap::Action::preconditionAdd(Precondition precondition) {
	preconditions.insert(precondition);
}
void Goap::Action::preconditionAdd(WorldState state, bool preconditionValue) {
	preconditionAdd(Precondition(state, preconditionValue));
}

void Goap::Action::effectAdd(Effect effect) {
	effects.insert(effect);
}
void Goap::Action::effectAdd(WorldState state, bool effectValue) {
	effectAdd(Effect(state, effectValue));
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

void Goap::Action::actionUpdate(Actor& actor) {
	execute(actor);
	evaluate(actor);
}
void Goap::Action::execute(Actor& actor) {
	std::invoke(executionFunction, actor);
}
void Goap::Action::evaluate(Actor& actor) {
	costSet(std::invoke(evaluationFunction, actor));
}