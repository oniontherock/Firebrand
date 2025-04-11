#include "FabrikResolver.hpp"
#include "FabrikJointUpdater.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <iostream>

void ProceduralAnimation::Fabrik::Resolver::limbUpdate(Limb& limb) {

	Joint& jointA = limb.jointA;
	Joint& jointB = limb.jointB;

	sf::Vector3f jointAxis = jointB.position - jointA.position;

	float jointAxisLen = sqrt((jointAxis.x * jointAxis.x) + (jointAxis.y * jointAxis.y) + (jointAxis.z * jointAxis.z));

	sf::Vector3f jointDirection = jointAxis / jointAxisLen;

	sf::Vector3f jointAxisRemainder = sf::Vector3f(0.f, 0.f, 0.f);

	// if joints are too close, move them both away by half of the distance needed
	if (jointAxisLen < limb.lengthConstraintMin) {
		jointAxisRemainder = jointDirection * (jointAxisLen - limb.lengthConstraintMin);
	}
	else if (jointAxisLen > limb.lengthConstraintMax) {
		jointAxisRemainder = jointDirection * (jointAxisLen - limb.lengthConstraintMax);
	}

	jointA.position += jointAxisRemainder / 2.f;
	jointB.position -= jointAxisRemainder / 2.f;

	jointA.positionPrev += jointAxisRemainder / 2.f;
	jointB.positionPrev -= jointAxisRemainder / 2.f;

	JointUpdater::jointUpdate(limb.jointA);
	JointUpdater::jointUpdate(limb.jointB);
}
