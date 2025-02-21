#ifndef __GOAP_TYPES_H__
#define __GOAP_TYPES_H__

#include <string>
#include <map>
#include "Blackboard/GoapBlackboard.hpp"
#include <functional>

namespace Goap {

	typedef std::string BlackboardKey;
	typedef std::any BlackboardValue;

	typedef std::function<bool(BlackboardValue)> Condition;

	typedef std::pair<BlackboardKey, Condition> Precondition;
	typedef std::map<BlackboardKey, Condition> PreconditionMap;

	typedef std::pair<BlackboardKey, BlackboardValue> Effect;
	typedef std::map<BlackboardKey, BlackboardValue> EffectMap;
}

#endif