#ifndef __EPA_COLLISION_HANDLER_H__
#define __EPA_COLLISION_HANDLER_H__

#include "../CollisionShape.hpp"

class EPACollisionHandler {

public:
	// returns the collision normal for two shapes with a given simplex (simplex should be obtained by previous usage of the GJKCollisionHandler)
	static sf::Vector2f collisionVectorGet(CollisionShape& shapeA, CollisionShape& shapeB, const std::vector<sf::Vector2f>& simplex);

};


#endif