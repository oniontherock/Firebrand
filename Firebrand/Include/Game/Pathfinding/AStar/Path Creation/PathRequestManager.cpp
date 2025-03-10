#include "PathRequestManager.hpp"
#include "../../../ACECS/ECSRegistry.hpp"

std::queue<PathRequestManager::PathRequestData> PathRequestManager::pathRequestQueue;
PathRequestManager::PathRequestData PathRequestManager::pathRequestCurrent = PathRequestManager::PathRequestData();

PathRequestManager::PathRequestData::PathRequestData(sf::Vector2f _pathStart, sf::Vector2f _pathEnd, EntityId _pathRequester) {
	pathStart = _pathStart;
	pathEnd = _pathEnd;
	pathRequester = _pathRequester;
}
PathRequestManager::PathRequestData::PathRequestData() :
	PathRequestData(sf::Vector2f(0, 0), sf::Vector2f(0, 0), 0)
{}


void PathRequestManager::pathRequest(sf::Vector2f pathStart, sf::Vector2f pathEnd, EntityId pathRequester) {
	// create new request
	PathRequestData requestNew = PathRequestData(pathStart, pathEnd, pathRequester);
	// add request to queue
	pathRequestQueue.push(requestNew);
}

void PathRequestManager::queueProcessNext() {
	if (!pathRequestQueue.empty()) {
		pathRequestCurrent = pathRequestQueue.front();
		pathRequestQueue.pop();
		
		pathProcessFinished(AStarPathfinder::pathGet(pathRequestCurrent.pathEnd, pathRequestCurrent.pathRequester), true);
	}
}

void PathRequestManager::pathProcessFinished(AStarPath path, bool success) {

	EntityId pathRequesterCurrentId = pathRequestCurrent.pathRequester;
	Entity& pathRequesterCurrent = EntityManager::entityGet(pathRequesterCurrentId);

	auto* eventPath = pathRequesterCurrent.entityEventAddAndGet<EntityEvents::EventPathRequestCompleted>();
	eventPath->path = path;
	eventPath->success = success;
}

