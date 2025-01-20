#ifndef __COLLISION_MATH_H__
#define __COLLISION_MATH_H__

#include "CollisionShape.hpp"
#include <SFML/Graphics.hpp>

namespace CollisionMath {
	sf::Vector2f supportGet(CollisionShape& shapeA, CollisionShape& shapeB, sf::Vector2f supportDirection);
}

#endif