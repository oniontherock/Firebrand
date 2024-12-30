#include "PathRequestManager.hpp"
#include "../../../ACECS/ECSRegistry.hpp"

bool PathRequestManager::isPathProcessing = false;
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

	queueProcessNext();
}

void PathRequestManager::queueProcessNext() {
	if (!isPathProcessing && !pathRequestQueue.empty()) {
		pathRequestCurrent = pathRequestQueue.front();
		pathRequestQueue.pop();
		isPathProcessing = true;
		//Path
	}
}

void PathRequestManager::pathProcessFinished(AStarPath path) {
	//EntityId pathRequesterCurrentId = pathRequestCurrent.pathRequester;
	//Entity& pathRequesterCurrent = EntityManager::entityGet(pathRequesterCurrentId);

	//auto* eventPath = pathRequesterCurrent.entityEventAddAndGet<EntityEvents::EventPath>();
	//eventPath->path = path;
	//eventPath->success = success;

	queueProcessNext();
}

