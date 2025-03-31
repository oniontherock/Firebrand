#ifndef __FABRIK_LIMB_H__
#define __FABRIK_LIMB_H__

#include "FabrikJoint.hpp"

namespace ProceduralAnimation {
	namespace Fabrik {
		struct Limb {
			// one of the two joints in this limb
			Joint jointA;
			// one of the two joints in this limb
			Joint jointB;

			// if the limb is not under stress it should attempt to move towards this length
			float lengthPreferred = 24;
			// the minimum distance there can be between both joints
			float lengthConstraintMin = 16;
			// the maximum distance there can be between both joints
			float lengthConstraintMax = 32;

			// a scale from 0.0 to 1.0 that determines how much both the joints move when resolving the connection,
			// a value of 0.5 means both joints move equally to resolve, 0.0 means jointB moves all the way, and 1.0 means jointA moves all the way
			float jointInfluence = 0.5f;

			// the strength of the limb, this affects how much control the limb has over it's joints' velocity.
			// note that, no matter the strength, joints will still always remain within the lengthConstraintMin to lengthConstraintMax range
			// 
			// a value of 1.0 means it has absolute control,
			// meaning it can completely change the velocity of the joint however it wants, and the joint is not affected by gravity at all
			// 
			// a value of 0.0 means it has absolutely no control,
			// meaning it cannot affect the velocity of the joints and they are completely affected by gravity, note that the limb still contains the joints between the min and max constraints
			float strength = 0.75f;
		};
	}
}

#endif