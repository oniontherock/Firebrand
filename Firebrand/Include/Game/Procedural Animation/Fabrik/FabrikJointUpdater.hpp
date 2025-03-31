#ifndef __FABRIK_JOINT_UPDATER_H__
#define __FABRIK_JOINT_UPDATER_H__

#include "FabrikLimb.hpp"
#include "FabrikJoint.hpp"
#include <vector>

namespace ProceduralAnimation {
	namespace Fabrik {
		namespace JointUpdater {

			// amount of gravity applied to joints per second
			constexpr sf::Vector3f jointGravityVector = sf::Vector3f(0.f, 0.f, 180.f);

			void jointPhysicsUpdate(Joint& joint, float delta);
			void jointUpdate(Joint& joint);
		};
	}
}

#endif