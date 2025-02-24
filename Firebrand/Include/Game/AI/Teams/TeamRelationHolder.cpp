#include "TeamRelationHolder.hpp"


std::map<std::pair<Teams::TeamId, Teams::TeamId>, Teams::TeamRelationValue> Teams::TeamRelationHolder::teamRelations{};

void Teams::TeamRelationHolder::teamRelationshipCreate(TeamId teamIdA, TeamId teamIdB, TeamRelationValue value) {
	teamRelations.insert({ std::pair(teamIdA, teamIdB), value });
}
void Teams::TeamRelationHolder::teamRelationshipCreate(TeamId teamIdA, TeamId teamIdB) {
	teamRelationshipCreate(teamIdA, teamIdB, 0.f);
}

void Teams::TeamRelationHolder::teamRelationSet(TeamId teamIdA, TeamId teamIdB, TeamRelationValue teamRelationNew) {
	teamRelations[std::pair(teamIdA, teamIdB)] = teamRelationNew;
}
Teams::TeamRelationValue Teams::TeamRelationHolder::teamRelationGet(TeamId teamIdA, TeamId teamIdB) {
	return teamRelations[std::pair(teamIdA, teamIdB)];
}
void Teams::TeamRelationHolder::teamRelationIncrement(TeamId teamIdA, TeamId teamIdB, TeamRelationValue teamRelationOffset) {
	teamRelationSet(teamIdA, teamIdB, teamRelationGet(teamIdA, teamIdB) + teamRelationOffset);
}
