#include "MovementPointHandler.hpp"

Movement::MovementPoint::MovementPoint(sf::Vector2f _vec, float _threshold, uint16_t _priority) {
	x = _vec.x;
	y = _vec.y;
	threshold = _threshold;
	priority = _priority;
}
Movement::MovementPoint::MovementPoint(sf::Vector2f _vec, float _threshold) :
	MovementPoint(_vec, _threshold, 50)
{}
Movement::MovementPoint::MovementPoint(sf::Vector2f _vec) :
	MovementPoint(_vec, 64.f, 50)
{}
Movement::MovementPoint::MovementPoint() {
}

bool Movement::MovementPoint::operator< (MovementPoint& other) const {
	return priority < other.priority;
}
bool Movement::MovementPoint::operator< (const MovementPoint& other) const {
	return priority < other.priority;
}

void Movement::MovementPointHandler::targetPointAdd(Movement::MovementPoint movementPoint) {
	targetPoints.push(movementPoint);
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
