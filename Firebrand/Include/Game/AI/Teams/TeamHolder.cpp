#include "TeamHolder.hpp"
#include <iterator>
#include <algorithm>
#include <Auxiliary/ConsoleHandler.hpp>
#include "TeamRegistry.hpp"

std::set<Teams::TeamId> Teams::TeamHolder::availableTeamIds{};
std::unordered_map<Teams::TeamId, Teams::TeamSet> Teams::TeamHolder::teamsUMap{};
std::unordered_map<EntityId, Teams::TeamId> Teams::TeamHolder::entityTeamUMap{};

void Teams::TeamHolder::initialize() {
	for (TeamId i = 0; i < maxTeamCount; i++) {
		availableTeamIds.insert(i);
	}

	teamCreateFromId(TeamId(TeamRegistry::Player));
	teamCreateFromId(TeamId(TeamRegistry::Flesh));
	teamCreateFromId(TeamId(TeamRegistry::Shepard));
}

void Teams::TeamHolder::teamCreateFromId(TeamId teamId) {
	if (!availableTeamIds.contains(teamId)) {
		ConsoleHandler::consolePrintErr("Team creation attempted from unavailable Id: " + std::to_string(teamId));
		return;
	}
	teamsUMap.insert({ teamId, TeamSet() });
}
Teams::TeamId Teams::TeamHolder::teamCreate() {
	TeamId minId = *availableTeamIds.begin();
	teamCreateFromId(minId);
	return minId;
}

void Teams::TeamHolder::teamAddEntity(TeamId teamId, EntityId entityId) {
	// if the teamId is in availableTeamIds then the team doesn't exist
	if (availableTeamIds.contains(teamId)) {
		teamCreateFromId(teamId);
		return;
	}
	teamsUMap[teamId].insert(entityId);
	entityTeamUMap.insert({ entityId, teamId });
}
void Teams::TeamHolder::teamRemoveEntity(TeamId teamId, EntityId entityId) {
	try {
		// if the teamId is in availableTeamIds then the team doesn't exist
		if (availableTeamIds.contains(teamId)) {
			throw std::string("Attempted to remove entity (Id=" + std::to_string(entityId) + ") from non-existant team: " + std::to_string(teamId));
		}
		if (!entityTeamHas(entityId)) {
			throw std::string("Attempted to remove team-less entity (Id=" + std::to_string(entityId) + ") from team: " + std::to_string(teamId));
		}
	}
	catch (std::string e) {
		ConsoleHandler::consolePrintErr(e);
	}
	teamsUMap[teamId].erase(entityId);
	entityTeamUMap.erase(entityId);
}
void Teams::TeamHolder::teamRemoveEntity(EntityId entityId) {
	try {
		if (!entityTeamHas(entityId)) {
			throw std::string("Attempted to remove team-less entity (Id=" + std::to_string(entityId) + ") from team");
		}
	}
	catch (std::string e) {
		ConsoleHandler::consolePrintErr(e);
	}
	teamsUMap[entityTeamGet(entityId)].erase(entityId);
	entityTeamUMap.erase(entityId);
}


bool Teams::TeamHolder::entityTeamHas(EntityId entityId) {
	return entityTeamUMap.contains(entityId);
}
Teams::TeamId Teams::TeamHolder::entityTeamGet(EntityId entityId) {
	if (!entityTeamHas(entityId)) {
		ConsoleHandler::consolePrintErr("Attempted to get entity's team when entity does not have a team");
		return UINT32_MAX;
	}
	return entityTeamUMap[entityId];
}
void Teams::TeamHolder::entityTeamSwitch(TeamId teamIdNew, EntityId entityId) {
	teamRemoveEntity(entityId);
	teamAddEntity(teamIdNew, entityId);
}
