#ifndef __COLLISION_MATH_H__
#define __COLLISION_MATH_H__

#include "CollisionShape.hpp"
#include <SFML/Graphics.hpp>

namespace CollisionMath {
	sf::Vector2f supportGet(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB, sf::Vector2f supportDirection);
}

#endif