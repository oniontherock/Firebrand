#include "CollisionMath.hpp"

sf::Vector2f CollisionMath::supportGet(CollisionShape& shapeA, CollisionShape& shapeB, sf::Vector2f supportDirection) {
	return shapeA.supportPointGet(supportDirection) - shapeB.supportPointGet(-supportDirection);
}

