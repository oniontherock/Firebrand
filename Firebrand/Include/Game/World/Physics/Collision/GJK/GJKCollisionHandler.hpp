#ifndef __GJK_COLLISION_HANDLER_H__
#define __GJK_COLLISION_HANDLER_H__

#include "CollisionShape.hpp"

class GJKCollisionHandler {
	
	static sf::Vector2f direction;

	static sf::Vector2f supportGet(CollisionShape& shapeA, CollisionShape& shapeB);
	
	static void directionSetToVec3(sf::Vector3f directionNew);

	static bool lineProcess(std::vector<sf::Vector2f>& simplex);
	static bool triangleProcess(std::vector<sf::Vector2f>& simplex);
	
	static bool simplexProcess(std::vector<sf::Vector2f>& simplex);
public:
	static bool collisionsCheck(CollisionShape& shapeA, CollisionShape& shapeB);
};


#endif