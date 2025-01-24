#ifndef __COLLISION_HANDLER_H__
#define __COLLISION_HANDLER_H__

#include "CollisionShape.hpp"

class CollisionHandler {
public:
	static bool collisionCheck(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB);
	// returns the collision vector of the last collision that was checked
	static sf::Vector2f collisionVectorGet(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB);
};


#endif