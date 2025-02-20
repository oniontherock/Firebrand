#include "GoapAction.hpp"

void Goap::Action::preconditionAdd(Precondition precondition) {
	preconditions.insert(precondition);
}
void Goap::Action::preconditionAdd(std::string preconditionName, bool preconditionValue) {
	preconditionAdd(Precondition(preconditionName, preconditionValue));
}

void Goap::Action::effectAdd(Effect effect) {
	effects.insert(effect);
}
void Goap::Action::effectAdd(std::string effectName, bool effectValue) {
	effectAdd(effectName, effectValue);
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