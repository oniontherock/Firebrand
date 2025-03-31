#ifndef __FABRIK_JOINT_H__
#define __FABRIK_JOINT_H__

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace ProceduralAnimation {
	namespace Fabrik {
		struct Joint {
			// position of this joint globally in the world
			sf::Vector3f position;
			// the previous position of the joint, used for physics
			sf::Vector3f positionPrev;

			sf::Vector3f acceleration;

			// applies a force to the joint
			void forceApply(sf::Vector3f force);
		};
	}
}

#endif