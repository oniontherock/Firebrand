#include "FabrikJoint.hpp"

void ProceduralAnimation::Fabrik::Joint::forceApply(sf::Vector3f force) {
	acceleration += force;
}
