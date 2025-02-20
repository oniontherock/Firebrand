#ifndef __GOAP_TYPES_H__
#define __GOAP_TYPES_H__

#include <string>
#include <map>

namespace Goap {
	enum class WorldState {
		IsThreatNear,
		IsHealthLow,
		HasWeapon,
		IsHungry,
		HasFood,
		IsAllyNear,
		IsLeaderNear,
		IsInBuilding,
	};

	typedef std::pair<WorldState, bool> Precondition;
	typedef std::map<WorldState, bool> PreconditionMap;

	typedef std::pair<WorldState, bool> Effect;
	typedef std::map<WorldState, bool> EffectMap;
}


#endif