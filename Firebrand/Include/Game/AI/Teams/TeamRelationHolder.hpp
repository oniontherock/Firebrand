#ifndef __TEAM_RELATION_HOLDER_H__
#define __TEAM_RELATION_HOLDER_H__

#include <vector>
#include <map>
#include <ECS/Entities/EntityManager.hpp>
#include <ECS/Entities/Entity.hpp>
#include "TeamHolder.hpp"

namespace Teams {

	enum class ThreatLevel {
		Enemy,
		Neutral,
		Ally,
	};

	// should always be between -100 and +100
	typedef float TeamRelationValue;

	struct TeamRelationHolder {
		static std::map<std::pair<TeamId, TeamId>, TeamRelationValue> teamRelations;

		static void teamRelationshipCreate(TeamId teamIdA, TeamId teamIdB, TeamRelationValue value);
		static void teamRelationshipCreate(TeamId teamIdA, TeamId teamIdB);

		static void teamRelationSet(TeamId teamIdA, TeamId teamIdB, TeamRelationValue teamRelationNew);
		static TeamRelationValue teamRelationGet(TeamId teamIdA, TeamId teamIdB);
		static void teamRelationIncrement(TeamId teamIdA, TeamId teamIdB, TeamRelationValue teamRelationOffset);
	};
}

#endif
