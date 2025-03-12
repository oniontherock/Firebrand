#include "MovementPointHandler.hpp"


bool Movement::MovementPoint::operator< (MovementPoint& other) {
	return priority < other.priority;
}
bool Movement::MovementPoint::operator< (const MovementPoint& other) {
	return priority < other.priority;
}

void Movement::MovementPointHandler::targetPointAdd(Movement::MovementPoint movementPoint) {
	auto itr = std::lower_bound(targetPoints.begin(), targetPoints.end(), movementPoint);
	targetPoints.insert(itr, movementPoint);
}
void Movement::MovementPointHandler::targetPointAdd(sf::Vector2f point, float threshold, uint16_t priority) {
	targetPointAdd(MovementPoint(point, threshold, priority));
}

void Movement::MovementPointHandler::avoidancePointAdd(Movement::MovementPoint movementPoint) {
	avoidancePoints.push_back(movementPoint);
}
void Movement::MovementPointHandler::avoidancePointAdd(sf::Vector2f point, float threshold, uint16_t priority) {
	avoidancePointAdd(MovementPoint(point, threshold, priority));
}
