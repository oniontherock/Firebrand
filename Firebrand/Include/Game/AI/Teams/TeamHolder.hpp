#ifndef __TEAM_HOLDER_H__
#define __TEAM_HOLDER_H__


#include <vector>
#include <unordered_map>
#include <ECS/Entities/EntityManager.hpp>
#include <ECS/Entities/Entity.hpp>

namespace TeamHolder {

	typedef uint32_t TeamId;
	typedef std::set<EntityId> TeamSet;

	constexpr TeamId maxTeamCount = 1000;

	extern std::set<TeamId> availableTeamIds;
	// this unordered_map contains a list of every team, and every entity in those teams,
	// we use an unordered_map instead of a vector so we can remove teams easier.
	extern std::unordered_map<TeamId, TeamSet> teamsUMap;
	// this unordered_map is used for getting what team an entity is on
	extern std::unordered_map<EntityId, TeamId> entityTeamUMap;

	void initialize();
	void teamCreateFromId(TeamId teamId);
	TeamId teamCreate();

	void teamAddEntity(TeamId teamId, EntityId entityId);
	void teamRemoveEntity(TeamId teamId, EntityId entityId);
	void teamRemoveEntity(EntityId entityId);

	bool entityTeamHas(EntityId entityId);
	TeamId entityTeamGet(EntityId entityId);
	void entityTeamSwitch(TeamId teamIdNew, EntityId entityId);
}

#endif
