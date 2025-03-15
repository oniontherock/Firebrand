#ifndef __MOVEMENT_POINT_HANDLER_H__
#define __MOVEMENT_POINT_HANDLER_H__

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <queue>

namespace Movement {

	// basically just a Vector2f, but it has a float called "threshold" which, depending on what the point is for,
	// can either be the distance an actor must get close to a point, or the distance an actor must stay away from a point
	//
	// also has a priority, which determines which points are moved towards first, or which points are avoided the most
	struct MovementPoint : sf::Vector2f {
		MovementPoint(sf::Vector2f _vec, float _threshold, uint16_t _priority);
		MovementPoint(sf::Vector2f _vec, float _threshold);
		MovementPoint(sf::Vector2f _vec);
		MovementPoint();

		float threshold = 16.f;
		uint16_t priority = 50;

		bool operator< (MovementPoint& other) const;
		bool operator< (const MovementPoint& other) const;
	};

	struct MovementPointHandler {
		// points the actor should move towards, the actor moves to the closest (or easiest) point first, and then goes towards the next point in the list
		std::priority_queue<MovementPoint> targetPoints;
		// points the actor should avoid going within the threshold of, the actor will avoid these points when pathfinding.
		std::vector<MovementPoint> avoidancePoints;

		void targetPointAdd(MovementPoint movementPoint);
		void targetPointAdd(sf::Vector2f point, float threshold, uint16_t priority);

		void avoidancePointAdd(MovementPoint movementPoint);
		void avoidancePointAdd(sf::Vector2f point, float threshold, uint16_t priority);
	};
}

#endif