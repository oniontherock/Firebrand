#ifndef __LIMB_DRAWER_H__
#define __LIMB_DRAWER_H__

#include "../Fabrik/FabrikLimb.hpp"
#include "LimbDrawingParameters.hpp"
#include <SFML/Graphics.hpp>

namespace ProceduralAnimation {
	namespace Drawing {
		namespace LimbDrawer {
			sf::Texture limbTextureGet(const Fabrik::Limb& limb);

			sf::Color jointColorGet(const Fabrik::Joint& joint);
			float jointSizeGet(const Fabrik::Joint& joint);

			sf::Color jointColorInterpolatedGet(const Fabrik::Joint& jointA, const Fabrik::Joint& jointB, float amount);
			float jointSizeInterpolatedGet(const Fabrik::Joint& jointA, const Fabrik::Joint& jointB, float amount);
		}
	}
}

#endif