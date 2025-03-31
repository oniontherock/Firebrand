#include "FabrikJointUpdater.hpp"
#include <Auxiliary/TimeHandler.hpp>

void ProceduralAnimation::Fabrik::JointUpdater::jointPhysicsUpdate(Joint& joint, float delta) {
	
	sf::Vector3f jointVelocity = (joint.position - joint.positionPrev) * 0.999f;

	joint.positionPrev = joint.position;

	joint.position += jointVelocity + joint.acceleration * delta * delta;
	
	joint.acceleration = sf::Vector3f(0, 0, 0);
}
void ProceduralAnimation::Fabrik::JointUpdater::jointUpdate(Joint& joint) {
	jointPhysicsUpdate(joint, TimeHandler::deltaSimulatedGet());
}
