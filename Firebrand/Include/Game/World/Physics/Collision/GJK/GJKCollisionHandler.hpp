#ifndef __GJK_COLLISION_HANDLER_H__
#define __GJK_COLLISION_HANDLER_H__

#include "../CollisionShape.hpp"

class CollisionHandler;

class GJKCollisionHandler {
	
	static sf::Vector2f direction;
	static std::vector<sf::Vector2f> simplex;

	static void directionSetToVec3(sf::Vector3f directionNew);

	static bool lineProcess();
	static bool triangleProcess();
	
	static bool simplexProcess();


public:
	static bool collisionsCheck(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB);
	
	friend class CollisionHandler;
};


#endif