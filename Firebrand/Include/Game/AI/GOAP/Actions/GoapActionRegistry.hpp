#ifndef __GOAP_ACTION_REGISTRY_H__
#define __GOAP_ACTION_REGISTRY_H__

#include "GoapAction.hpp"
#include <map>
#include <string>


namespace Goap {
	namespace ActionRegistry {
		extern std::map<ActionName, Action> actions;

		Action& actionAdd(ActionName actionName);

		void actionsRegister();
	}
}

#endif
