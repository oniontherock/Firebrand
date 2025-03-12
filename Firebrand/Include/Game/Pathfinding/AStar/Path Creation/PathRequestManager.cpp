#include "PathRequestManager.hpp"
#include "../../../ACECS/ECSRegistry.hpp"

std::queue<PathRequestManager::PathRequestData> PathRequestManager::pathRequestQueue;
PathRequestManager::PathRequestData PathRequestManager::pathRequestCurrent = PathRequestManager::PathRequestData();

void PathRequestManager::pathRequest(sf::Vector2f pathStart, sf::Vector2f pathEnd, EntityId pathRequester) {
	// create new request
	PathRequestData requestNew = PathRequestData(pathStart, pathEnd, pathRequester, false);
	// add request to queue
	pathRequestQueue.push(requestNew);
}
void PathRequestManager::pathRequest(sf::Vector2f pathStart, sf::Vector2f pathEnd, EntityId pathRequester, AStarPathfinder::SpecializationFunction specializationFunction) {
	// create new request
	PathRequestData requestNew = PathRequestData(pathStart, pathEnd, pathRequester, true, specializationFunction);
	// add request to queue
	pathRequestQueue.push(requestNew);
}

void PathRequestManager::queueProcessNext() {
	if (!pathRequestQueue.empty()) {
		pathRequestCurrent = pathRequestQueue.front();
		pathRequestQueue.pop();
		
		if (pathRequestCurrent.isSpecialized) {
			pathProcessFinished(AStarPathfinder::pathGet(pathRequestCurrent.pathEnd, pathRequestCurrent.pathRequester, pathRequestCurrent.specializationFunction), true);
		}
		else {
			pathProcessFinished(AStarPathfinder::pathGet(pathRequestCurrent.pathEnd, pathRequestCurrent.pathRequester), true);
		}
	}
}

void PathRequestManager::pathProcessFinished(AStarPath path, bool success) {

	EntityId pathRequesterCurrentId = pathRequestCurrent.pathRequester;
	Entity& pathRequesterCurrent = EntityManager::entityGet(pathRequesterCurrentId);

	auto* eventPath = pathRequesterCurrent.entityEventAddAndGet<EntityEvents::EventPathRequestCompleted>();
	eventPath->path = path;
	eventPath->success = success;
}

