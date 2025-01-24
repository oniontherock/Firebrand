#include "CollisionHandler.hpp"
#include "EPA/EPACollisionHandler.hpp"
#include "GJK/GJKCollisionHandler.hpp"

bool CollisionHandler::collisionCheck(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB) {
	return GJKCollisionHandler::collisionsCheck(shapeA, shapeB);
}
sf::Vector2f CollisionHandler::collisionVectorGet(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB) {
	return EPACollisionHandler::collisionVectorGet(shapeA, shapeB, GJKCollisionHandler::simplex);
}
