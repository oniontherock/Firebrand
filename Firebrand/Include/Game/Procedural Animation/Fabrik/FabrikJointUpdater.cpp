#include "FabrikJointUpdater.hpp"
#include <Auxiliary/TimeHandler.hpp>

void ProceduralAnimation::Fabrik::JointUpdater::jointPhysicsUpdate(Joint& joint, float delta) {

	sf::Vector3f jointVelocity = (joint.position - joint.positionPrev) * 0.9f;

	joint.positionPrev = joint.position;

	joint.position += jointVelocity + joint.acceleration * delta * delta;
	
	joint.acceleration = sf::Vector3f(0, 0, 0);
}
void ProceduralAnimation::Fabrik::JointUpdater::jointHeightLimit(Joint& joint) {
	if (joint.position.z < minHeight) {
		joint.position.z = minHeight;
	}
	else if (joint.position.z > maxHeight) {
		joint.position.z = maxHeight;
	}
}
void ProceduralAnimation::Fabrik::JointUpdater::jointUpdate(Joint& joint) {

	if (!joint.hasUpdated) {
		joint.positionPrev = joint.position;
		joint.hasUpdated = true;
	}

	joint.forceApply(jointGravityVector);
	jointPhysicsUpdate(joint, TimeHandler::deltaSimulatedGet());
	jointHeightLimit(joint);
}
