#include "CollisionMath.hpp"

sf::Vector2f CollisionMath::supportGet(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB, sf::Vector2f supportDirection) {
	return shapeA->supportPointGet(supportDirection) - shapeB->supportPointGet(-supportDirection);
}

