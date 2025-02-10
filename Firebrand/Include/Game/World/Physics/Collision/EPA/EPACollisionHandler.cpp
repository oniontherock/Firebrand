#include "../CollisionMath.hpp"
#include "Auxiliary/Math.hpp"
#include "Auxiliary/VectorMath.hpp"
#include "EPACollisionHandler.hpp"

sf::Vector2f EPACollisionHandler::collisionVectorGet(CollisionShapeBase* shapeA, CollisionShapeBase* shapeB, const std::vector<sf::Vector2f>& simplex) {

	uint16_t minInd = 0;
	float minDist = INFINITY;
	sf::Vector2f minNormal;

	std::vector<sf::Vector2f> polytope = simplex;

	while (minDist >= INFINITY) {
		for (uint16_t i = 0; i < polytope.size(); i++) {
			uint16_t j = (uint16_t(i + 1)) % polytope.size();

			sf::Vector2f vertexI = polytope[i];
			sf::Vector2f vertexJ = polytope[j];

			sf::Vector2f IJAxis = vertexJ - vertexI;

			sf::Vector2f normal;
			if (Vector2fMath::lengthSqrd(IJAxis) <= 0.001f) {
				normal.x = 1.f;
			}
			else {
				normal = Vector2fMath::normalize(IJAxis.y, -IJAxis.x);
			}
			float normalDot = Vector2fMath::dot(normal, vertexI);

			// make sure the normal faces the correct direction, if not, flip it and the normalDot
			if (normalDot < 0.f) {
				normalDot *= -1.f;
				normal *= -1.f;
			}

			if (normalDot < minDist) {
				minDist = normalDot;
				minNormal = normal;
				minInd = j;
			}
		}

		sf::Vector2f supportPoint = CollisionMath::supportGet(shapeA, shapeB, minNormal);
		float supportDot = Vector2fMath::dot(minNormal, supportPoint);

		if (abs(supportDot - minDist) > 0.001f) {
			minDist = INFINITY;
			polytope.insert(polytope.begin() + minInd, supportPoint);
		}
	}

	return minNormal * (minDist + 0.001f);
}
