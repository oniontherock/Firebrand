#ifndef __FABRIK_LIMB_H__
#define __FABRIK_LIMB_H__

#include "FabrikJoint.hpp"

namespace ProceduralAnimation {
	namespace Fabrik {
		struct Limb {
			// if the limb is not under stress it should attempt to move towards this length
			float lengthPreferred = 24;
			// the minimum distance there can be between both joints
			float lengthConstraintMin = 16;
			// the maximum distance there can be between both joints
			float lengthConstraintMax = 32;

			// one of the two joints in this limb
			Joint jointA;
			// one of the two joints in this limb
			Joint jointB;
		};
	}
}

#endif