#ifndef __GOAP_ACTION_H__
#define __GOAP_ACTION_H__

#include "../Actors/GoapActor.hpp"
#include "../GoapTypes.hpp"
#include <algorithm>
#include <functional>
#include <string>

namespace Goap {

	typedef std::string ActionName;
	typedef float ActionCost;
	typedef std::function<void(Actor&)> ActionExecutionFunction;
	typedef std::function<ActionCost(Actor&)> ActionEvaluationFunction;

	struct Action {

		PreconditionMap preconditions;
		EffectMap effects;

		ActionCost cost = 50.f;

		// adds a new precondition required for this action, so for example, a "ShootGun" action has the precondition of "HasGun" and "HasBullets"
		void preconditionAdd(Precondition precondition);
		// adds a new precondition required for this action, so for example, a "ShootGun" action has the precondition of "HasGun" and "HasBullets"
		void preconditionAdd(std::string preconditionName, bool preconditionValue);

		// adds a new effect that this action causes, so for example, an "EatFood" action has the effect of "IsHungry" being false
		void effectAdd(Effect effect);
		// adds a new effect that this action causes, so for example, an "EatFood" action has the effect of "IsHungry" being false
		void effectAdd(std::string effectName, bool effectValue);
		
		// sets the cost of this action, used when creating plans to determine the best action for the given scenerio,
		// later the cost system will be more complicated and dynamic but for now it is just a simple value
		void costSet(ActionCost costNew);

		// sets the action function to functionNew
		void executionFunctionSet(ActionExecutionFunction function);
		void evaluationFunctionSet(ActionEvaluationFunction function);

		void actionUpdate(Actor& actor);
	private:

		ActionExecutionFunction executionFunction;
		ActionEvaluationFunction evaluationFunction;

		void evaluate(Actor& actor);
		void execute(Actor& actor);
	};
}

#endif
