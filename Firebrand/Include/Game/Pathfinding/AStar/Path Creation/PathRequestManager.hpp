#ifndef __PATH_REQUEST_MANAGER_H__
#define __PATH_REQUEST_MANAGER_H__

#include <Auxiliary/TimeHandler.hpp>
#include <SFML/System/Vector2.hpp>
#include "AStarPathfinder.hpp"
#include <ECS/TypeDefinitions.hpp>
#include <queue>

class PathRequestManager {

	struct PathRequestData {

		sf::Vector2f pathStart;
		sf::Vector2f pathEnd;
		EntityId pathRequester = 0;

		bool isSpecialized = false;
		AStarPathfinder::SpecializationFunction specializationFunction;
	};

	// queue of path requests to process
	static std::queue<PathRequestData> pathRequestQueue;
	// current path request to process
	static PathRequestData pathRequestCurrent;
public:
	// processes next path request
	static void queueProcessNext();
	// sends an event to the current path requester, containing the path and whether or not the pathfinding was successful
	static void pathProcessFinished(AStarPath path, bool success);

	// request a path to be made
	static void pathRequest(sf::Vector2f pathStart, sf::Vector2f pathEnd, EntityId pathRequester);
	// request a path to be made
	static void pathRequest(sf::Vector2f pathStart, sf::Vector2f pathEnd, EntityId pathRequester, AStarPathfinder::SpecializationFunction specializationFunction);
};

#endif