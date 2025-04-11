#include "LimbDrawer.hpp"
#include "../include/Common/ColorConverter.hpp"

sf::Texture ProceduralAnimation::Drawing::LimbDrawer::limbTextureGet(const Fabrik::Limb& limb) {

	const ProceduralAnimation::Fabrik::Joint* jointA = &limb.jointA;
	const ProceduralAnimation::Fabrik::Joint* jointB = &limb.jointB;

	if (jointA->position.z > jointB->position.z) {
		jointB = &limb.jointA;
		jointA = &limb.jointB;
	}

	// for now we just stay each joint in any limb has a radius of 16.f
	const float limbJointsSize = 2.f;

	constexpr sf::Vector2u renderTextureSize = sf::Vector2u(256u, 256u);
	constexpr sf::Vector2f renderTextureHalfSize = sf::Vector2f(renderTextureSize / 2u);

	sf::RenderTexture renderTexture(renderTextureSize);

	// position of jointA in the world
	sf::Vector3f jointAPosition = jointA->position;
	// position of jointB in the world
	sf::Vector3f jointBPosition = jointB->position;

	// the point between the positions of jointA and jointB
	sf::Vector3f limbCenterPoint = (jointAPosition + jointBPosition) / 2.f;

	// position of jointA localized to the center of the limb
	sf::Vector3f jointAPositionLocal = jointA->position - limbCenterPoint;
	// position of jointB localized to the center of the limb
	sf::Vector3f jointBPositionLocal = jointB->position - limbCenterPoint;

	sf::Vector3f jointAxis = jointBPositionLocal - jointAPositionLocal;

	renderTexture.clear(sf::Color::Transparent);

	for (float lerpVal = 0.f; lerpVal <= 1.f; lerpVal += 0.1f) {
		sf::CircleShape circleShape;

		float size = jointSizeInterpolatedGet(*jointA, *jointB, lerpVal);
		circleShape.setRadius(size);

		circleShape.setOrigin(sf::Vector2f(size, size) / 2.f);

		sf::Color color = jointColorInterpolatedGet(*jointA, *jointB, lerpVal);
		circleShape.setFillColor(color);

		circleShape.setPosition((sf::Vector2f(jointAPositionLocal.x, jointAPositionLocal.y) + (sf::Vector2f(jointAxis.x, jointAxis.y) * lerpVal)) + renderTextureHalfSize);
		renderTexture.draw(circleShape);
	}

	renderTexture.display();

	return sf::Texture(renderTexture.getTexture());
}

sf::Color ProceduralAnimation::Drawing::LimbDrawer::jointColorGet(const Fabrik::Joint& joint) {
	
	// temporary joint color variable, just use this until individual color data for each joint is set up
	const sf::Color jointColorDefault = sf::Color(127, 50, 25);

	sf::Color jointColorAsHSV = ColorConverter::RgbToHsv(jointColorDefault);

	// joint z level scaled between 0.0 and 1.0 relative to the maxDrawHeight
	double valueScaled = double(joint.position.z) / double(LimbDrawingParameters::maxDrawHeight);
	// set the joint color's value (HSV.v) as the scaled value, rescaled between 0 and 255
	jointColorAsHSV.b = uint8_t(valueScaled * 255.0);

	return ColorConverter::HsvToRgb(jointColorAsHSV);
}
float ProceduralAnimation::Drawing::LimbDrawer::jointSizeGet(const Fabrik::Joint& joint) {
	// temporary joint size variable, just use this until individual size data for each joint is set up
	const float jointSizeDefault = 16.f;

	double valueScaled = (double(joint.position.z) / double(LimbDrawingParameters::maxDrawHeight)) + 0.5f;

	return jointSizeDefault * valueScaled;
}

sf::Color ProceduralAnimation::Drawing::LimbDrawer::jointColorInterpolatedGet(const Fabrik::Joint& jointA, const Fabrik::Joint& jointB, float amount) {
	sf::Color colorA = jointColorGet(jointA);
	sf::Color colorB = jointColorGet(jointB);

	int16_t colorAxisR = (colorB.r - colorA.r) * amount;
	int16_t colorAxisG = (colorB.g - colorA.g) * amount;
	int16_t colorAxisB = (colorB.b - colorA.b) * amount;
	int16_t colorAxisA = (colorB.a - colorA.a) * amount;

	return sf::Color(colorA.r + colorAxisR, colorA.g + colorAxisG, colorA.b + colorAxisB, colorA.a + colorAxisA);
}
float ProceduralAnimation::Drawing::LimbDrawer::jointSizeInterpolatedGet(const Fabrik::Joint& jointA, const Fabrik::Joint& jointB, float amount) {

	float sizeA = jointSizeGet(jointA);
	float sizeB = jointSizeGet(jointB);

	float sizeAxis = (sizeB - sizeA) * amount;

	return sizeA + sizeAxis;
}
