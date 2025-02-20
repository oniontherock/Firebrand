#ifndef __GOAP_TYPES_H__
#define __GOAP_TYPES_H__

#include <string>
#include <map>

namespace Goap {
	typedef std::pair<std::string, bool> Precondition;
	typedef std::map<std::string, bool> PreconditionMap;

	typedef std::pair<std::string, bool> Effect;
	typedef std::map<std::string, bool> EffectMap;
}


#endif