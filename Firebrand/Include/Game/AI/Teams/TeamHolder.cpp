#include "TeamHolder.hpp"
#include <iterator>
#include <algorithm>
#include <Auxiliary/ConsoleHandler.hpp>

std::set<TeamHolder::TeamId> TeamHolder::availableTeamIds{};
std::unordered_map<TeamHolder::TeamId, TeamHolder::TeamSet> TeamHolder::teamsUMap{};
std::unordered_map<EntityId, TeamHolder::TeamId> TeamHolder::entityTeamUMap{};

void TeamHolder::initialize() {
	for (TeamId i = 0; i < maxTeamCount; i++) {
		availableTeamIds.insert(i);
	}
}

void TeamHolder::teamCreateFromId(TeamId teamId) {
	if (!availableTeamIds.contains(teamId)) {
		ConsoleHandler::consolePrintErr("Team creation attempted from unavailable Id: " + std::to_string(teamId));
		return;
	}
	teamsUMap.insert({ teamId, TeamSet() });
}
TeamHolder::TeamId TeamHolder::teamCreate() {
	TeamId minId = *availableTeamIds.begin();
	teamCreateFromId(minId);
	return minId;
}

void TeamHolder::teamAddEntity(TeamId teamId, EntityId entityId) {
	// if the teamId is in availableTeamIds then the team doesn't exist
	if (availableTeamIds.contains(teamId)) {
		ConsoleHandler::consolePrintErr("Attempted to add entity (Id=" + std::to_string(entityId) + ") to non-existant team: " + std::to_string(teamId));
		return;
	}
	teamsUMap[teamId].insert(entityId);
	entityTeamUMap.insert({ entityId, teamId });
}
void TeamHolder::teamRemoveEntity(TeamId teamId, EntityId entityId) {
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
void TeamHolder::teamRemoveEntity(EntityId entityId) {
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


bool TeamHolder::entityTeamHas(EntityId entityId) {
	return entityTeamUMap.contains(entityId);
}
TeamHolder::TeamId TeamHolder::entityTeamGet(EntityId entityId) {
	if (!entityTeamHas(entityId)) {
		ConsoleHandler::consolePrintErr("Attempted to get entity's team when entity does not have a team");
		return UINT32_MAX;
	}
	return entityTeamUMap[entityId];
}
void TeamHolder::entityTeamSwitch(TeamId teamIdNew, EntityId entityId) {
	teamRemoveEntity(entityId);
	teamAddEntity(teamIdNew, entityId);
}
