#ifndef __COLLISION_HANDLER_H__
#define __COLLISION_HANDLER_H__

#include "CollisionShape.hpp"

class CollisionHandler {
public:
	static bool collisionCheck(CollisionShape& shapeA, CollisionShape& shapeB);
	// returns the collision vector of the last collision that was checked
	static sf::Vector2f collisionVectorGet(CollisionShape& shapeA, CollisionShape& shapeB);
};


#endif