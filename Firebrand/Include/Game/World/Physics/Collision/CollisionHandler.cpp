#include "CollisionHandler.hpp"
#include "EPA/EPACollisionHandler.hpp"
#include "GJK/GJKCollisionHandler.hpp"

bool CollisionHandler::collisionCheck(CollisionShape& shapeA, CollisionShape& shapeB) {
	return GJKCollisionHandler::collisionsCheck(shapeA, shapeB);
}
sf::Vector2f CollisionHandler::collisionVectorGet(CollisionShape& shapeA, CollisionShape& shapeB) {
	return EPACollisionHandler::collisionVectorGet(shapeA, shapeB, GJKCollisionHandler::simplex);
}
