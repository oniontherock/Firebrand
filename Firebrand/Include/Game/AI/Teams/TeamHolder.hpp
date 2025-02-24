#ifndef __TEAM_HOLDER_H__
#define __TEAM_HOLDER_H__

#include <vector>
#include <unordered_map>
#include <ECS/Entities/EntityManager.hpp>
#include <ECS/Entities/Entity.hpp>

namespace Teams {

	typedef uint32_t TeamId;
	typedef std::set<EntityId> TeamSet;

	constexpr TeamId maxTeamCount = 1000;

	struct TeamHolder {

		static std::set<TeamId> availableTeamIds;
		// this unordered_map contains a list of every team, and every entity in those teams,
		// we use an unordered_map instead of a vector so we can remove teams easier.
		static std::unordered_map<TeamId, TeamSet> teamsUMap;
		// this unordered_map is used for getting what team an entity is on
		static std::unordered_map<EntityId, TeamId> entityTeamUMap;

		static void initialize();
		static void teamCreateFromId(TeamId teamId);
		static TeamId teamCreate();

		static void teamAddEntity(TeamId teamId, EntityId entityId);
		static void teamRemoveEntity(TeamId teamId, EntityId entityId);
		static void teamRemoveEntity(EntityId entityId);

		static bool entityTeamHas(EntityId entityId);
		static TeamId entityTeamGet(EntityId entityId);
		static void entityTeamSwitch(TeamId teamIdNew, EntityId entityId);
	};
}

#endif
