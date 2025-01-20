#include "../CollisionMath.hpp"
#include "Auxiliary/Math.hpp"
#include "Auxiliary/VectorMath.hpp"
#include "GJKCollisionHandler.hpp"

#define ORIGIN sf::Vector2f(0, 0)

sf::Vector2f GJKCollisionHandler::direction(0, 0);
std::vector<sf::Vector2f> GJKCollisionHandler::simplex{};

void GJKCollisionHandler::directionSetToVec3(sf::Vector3f directionNew) {
	direction.x = directionNew.x;
	direction.y = directionNew.y;
}

bool GJKCollisionHandler::lineProcess() {
	sf::Vector2f B = simplex[0];
	sf::Vector2f A = simplex[1];

	sf::Vector3f AB = sf::Vector3f(B.x - A.x, B.y - A.y, 0);
	sf::Vector3f AO = sf::Vector3f(ORIGIN.x - A.x, ORIGIN.y - A.y, 0);

	sf::Vector3f ABPerp = Vector3fMath::tripleProduct(AB, AO, AB);

	directionSetToVec3(ABPerp);

	// TODO: add handling for edge cases where the origin lies on the edge of the line

	return false;
}
bool GJKCollisionHandler::triangleProcess() {
	sf::Vector2f C = simplex[0];
	sf::Vector2f B = simplex[1];
	sf::Vector2f A = simplex[2];

	sf::Vector3f AB = sf::Vector3f(B.x - A.x, B.y - A.y, 0);
	sf::Vector3f AC = sf::Vector3f(C.x - A.x, C.y - A.y, 0);
	sf::Vector3f AO = sf::Vector3f(ORIGIN.x - A.x, ORIGIN.y - A.y, 0);

	sf::Vector3f ABPerp = Vector3fMath::tripleProduct(AC, AB, AB);
	sf::Vector3f ACPerp = Vector3fMath::tripleProduct(AB, AC, AC);

	if (Vector3fMath::dot(ABPerp, AO) > 0.f) {
		// remove C from simplex
		simplex.erase(simplex.begin() + 0);
		directionSetToVec3(ABPerp);
		return false;
	}
	else if (Vector3fMath::dot(ACPerp, AO) > 0.f) {
		// remove B from simplex
		simplex.erase(simplex.begin() + 1);
		directionSetToVec3(ACPerp);
		return false;
	}
	return true;
}

bool GJKCollisionHandler::simplexProcess() {
	if (simplex.size() == 2) {
		return lineProcess();
	}
	return triangleProcess();
}

bool GJKCollisionHandler::collisionsCheck(CollisionShape& shapeA, CollisionShape& shapeB) {

	direction = Vector2fMath::dir(shapeA.centerGet(), shapeB.centerGet());

	simplex = { CollisionMath::supportGet(shapeA, shapeB, direction) };

	direction = Vector2fMath::dir(simplex[0], ORIGIN);

	while (true) {
		sf::Vector2f pointA = CollisionMath::supportGet(shapeA, shapeB, direction);

		if (Vector2fMath::dot(pointA, direction) < 0.f) return false;

		simplex.push_back(pointA);
		if (simplexProcess()) {
			return true;
		}
	}
}


