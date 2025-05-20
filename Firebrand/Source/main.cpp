#include "../ACECS/ACECS.hpp"
#include "../Include/Game/AI/GOAP/Planner/GoapPlanner.hpp"
#include "../Include/Game/GameData.hpp"
#include "../Include/Game/World/Physics/Collision/CollisionHandler.hpp"
#include "Auxiliary/ConsoleHandler.hpp"
#include "Graphics/WindowHolder.hpp"
#include "Input/Input Events/InputManager.hpp"
#include "Input/InputInterface.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include <fstream>
#include <iostream>
#include <Saving/SaveHandler.hpp>
#include <SFML/Window.hpp>

typedef std::vector<sf::Vector3f> JointVector;
typedef std::vector<float> JointDistanceVector;
typedef std::vector<std::pair<float, float>> JointAngleConstraintVector;

static void forwardReach(JointVector& joints, JointDistanceVector& jointDistances, JointAngleConstraintVector& jointAngleConstraints, sf::Vector3f target) {

	// we set the end effector of the joint chain to the target
	joints[joints.size() - 1] = target;

	// iterate backwards over every joint except for the end effector
	for (int16_t i = joints.size() - 1; i >= 1; i--) {



		//float rootAngle = i >= 2 ? (rootAngle = Vector2fMath::angle(joints[uint16_t(i - 2)], joints[uint16_t(i - 1)])) : 0.f;

		//float differenceAngleRaw = Vector2fMath::angle(joints[uint16_t(i - 1)], joints[i]) - rootAngle;

		//if (differenceAngleRaw < -Mathf::PI) differenceAngleRaw += Mathf::TAU;
		//if (differenceAngleRaw > +Mathf::PI) differenceAngleRaw -= Mathf::TAU;

		//float differenceAngle = Mathf::clamp(differenceAngleRaw,
		//	jointAngleConstraints[uint16_t(i - 1)].first * Mathf::PI / 180.f,
		//	jointAngleConstraints[uint16_t(i - 1)].second * Mathf::PI / 180.f
		//);

		//float angle = rootAngle + differenceAngle;

		//angle += Mathf::PI;

		//// wrap differenceAngleRaw between -PI and +PI
		//if (angle < -Mathf::PI) angle += Mathf::TAU;
		//if (angle > +Mathf::PI) angle -= Mathf::TAU;

		float angleNoConstraints = Vector2fMath::angle(joints[uint16_t(i)].x, joints[uint16_t(i)].y, joints[uint16_t(i - 1)].x, joints[uint16_t(i - 1)].y);

		joints[uint16_t(i - 1)].x = joints[i].x + Vector2fMath::rotate(sf::Vector2f(jointDistances[i - 1], 0.f), angleNoConstraints).x;
		joints[uint16_t(i - 1)].y = joints[i].y + Vector2fMath::rotate(sf::Vector2f(jointDistances[i - 1], 0.f), angleNoConstraints).y;
	}
}
static void backwardReach(JointVector& joints, JointDistanceVector& jointDistances, JointAngleConstraintVector& jointAngleConstraints, sf::Vector3f base) {
	// we set the end effector of the joint chain to the target
	joints[0] = base;

	float rootAngle = 0.f;

	// iterate backwards over every joint except for the end effector
	for (int16_t i = 0; i < joints.size() - 1; i++) {

		//float differenceAngleRaw = Vector2fMath::angle(joints[i], joints[uint16_t(i + 1)]) - rootAngle;

		//// wrap differenceAngleRaw between -PI and +PI
		//if (differenceAngleRaw < -Mathf::PI) differenceAngleRaw += Mathf::TAU;
		//if (differenceAngleRaw > +Mathf::PI) differenceAngleRaw -= Mathf::TAU;
		//
		//float differenceAngle = Mathf::clamp(
		//	differenceAngleRaw,
		//	jointAngleConstraints[i].first * Mathf::PI / 180.f,
		//	jointAngleConstraints[i].second * Mathf::PI / 180.f
		//);

		//float angle = rootAngle + differenceAngle;

		float angleNoConstraints = Vector2fMath::angle(joints[i].x, joints[i].y, joints[uint16_t(i + 1)].x, joints[uint16_t(i + 1)].y);

		joints[uint16_t(i + 1)].x = joints[i].x + Vector2fMath::rotate(sf::Vector2f(jointDistances[i], 0.f), angleNoConstraints).x;
		joints[uint16_t(i + 1)].y = joints[i].y + Vector2fMath::rotate(sf::Vector2f(jointDistances[i], 0.f), angleNoConstraints).y;

		//rootAngle = differenceAngleRaw;
	}
}

static float fabrikRun(JointVector& joints, JointDistanceVector& jointDistances, JointAngleConstraintVector& jointAngleConstraints, sf::Vector3f base, sf::Vector3f target) {

	float baseToTargetDistSqrd = Vector3fMath::distSqrd(base, target);

	float jointDistanceTotal = 0.f;
	for (uint16_t i = 0; i < jointDistances.size(); i++) {
		jointDistanceTotal += jointDistances[i];
	}

	// if the target is too far to be reachable by the joint chain, we just run FABRIK once and return
	if (baseToTargetDistSqrd > (jointDistanceTotal * jointDistanceTotal)) {
		forwardReach(joints, jointDistances, jointAngleConstraints, target);
		backwardReach(joints, jointDistances, jointAngleConstraints, base);
		return 0.f;
	}

	for (uint16_t i = 0; i < 128; i++) {
		forwardReach(joints, jointDistances, jointAngleConstraints, target);
		backwardReach(joints, jointDistances, jointAngleConstraints, base);

		if (Vector3fMath::distSqrd(joints[joints.size() - 1].x, joints[joints.size() - 1].y, joints[joints.size() - 1].z, target.x, target.y, 0) < 1.f * 1.f) {
			break;
		}
	}

	return Vector3fMath::distSqrd(joints[joints.size() - 1].x, joints[joints.size() - 1].y, joints[joints.size() - 1].z, target.x, target.y, 0);
}

const float lerpAmount = 0.5f;

static void fabrikUpdate(JointVector& joints, JointDistanceVector& jointDistances, JointAngleConstraintVector& jointAngleConstraints, sf::Vector3f base, sf::Vector3f target) {
	JointVector jointsOld = joints;

	float dist = fabrikRun(joints, jointDistances, jointAngleConstraints, base, target);

	if (dist > 1.f * 1.f) {

		joints = jointsOld;

		sf::Vector3f targetNew = base + (Vector3fMath::dir(base, target) * Vector3fMath::dist(base, joints[joints.size() - 1]));

		fabrikRun(joints, jointDistances, jointAngleConstraints, base, targetNew);
	}

	JointVector jointsNew(joints.size());

	for (uint16_t i = 0; i < joints.size(); i++) {

		sf::Vector3f axis = Vector3fMath::axis(jointsOld[i], joints[i]);

		jointsNew[i] = jointsOld[i] + (axis * lerpAmount);
	}

	joints = jointsNew;
}

static sf::Texture fabrikDraw() {
	
	sf::RenderTexture fabrikEndTexture(sf::Vector2u(1280, 720));

	fabrikEndTexture.clear(sf::Color::Transparent);

	static JointVector joints(5);
	static JointDistanceVector jointDistances(4, 64);
	static JointAngleConstraintVector jointAngleConstraints(4, { -90.f, 90.f });

	jointAngleConstraints[0].first = -180.f;
	jointAngleConstraints[0].second = 180.f;

	constexpr sf::Vector3f base = sf::Vector3f(640, 360, 0);

	static sf::Vector2i mousePos = sf::Mouse::getPosition(WindowHolder::windowGet());
	static sf::Vector3f target = sf::Vector3f(mousePos.x, mousePos.y, 64);

	target += Vector3fMath::axis(target.x, target.y, 0, mousePos.x, mousePos.y, 0) * 0.025f;

	fabrikUpdate(joints, jointDistances, jointAngleConstraints, base, target);

	constexpr float limbDrawWidth = 4.f;

	for (uint16_t i = 0; i < jointDistances.size(); i++) {
		sf::RectangleShape limbShape;

		limbShape.setSize(sf::Vector2f(jointDistances[i], limbDrawWidth));
		limbShape.setFillColor(sf::Color::White);

		limbShape.setOrigin(sf::Vector2f(0, limbDrawWidth / 2.f));

		sf::Vector2f jointA = sf::Vector2f(joints[i].x, joints[i].y);
		sf::Vector2f jointB = sf::Vector2f(joints[uint16_t(i + 1)].x, joints[uint16_t(i + 1)].y);

		limbShape.setPosition(jointA);

		sf::Vector2f limbAxis = jointB - jointA;

		float limbAngle = atan2(limbAxis.y, limbAxis.x);

		limbShape.setRotation(sf::radians(limbAngle));

		fabrikEndTexture.draw(limbShape);
	}

	constexpr float jointDrawRadius = 2.f;

	for (uint16_t i = 0; i < joints.size(); i++) {
		sf::CircleShape jointShape(jointDrawRadius);

		jointShape.setPosition(sf::Vector2f(joints[i].x, joints[i].y));
		jointShape.setFillColor(sf::Color(255, 0, 0, 225));
		jointShape.setOrigin(sf::Vector2f(jointDrawRadius, jointDrawRadius));

		fabrikEndTexture.draw(jointShape);
	}

	constexpr float angleConstraintDrawLength = 32.f;
	constexpr float angleConstraintDrawWidth = 2.f;
	sf::RectangleShape angleConstraintShape;

	angleConstraintShape.setSize(sf::Vector2f(angleConstraintDrawLength, angleConstraintDrawWidth));
	angleConstraintShape.setFillColor(sf::Color(255, 255, 0, 125));
	angleConstraintShape.setOrigin(sf::Vector2f(0, angleConstraintDrawWidth / 2.f));

	for (uint16_t i = 0; i < jointAngleConstraints.size(); i++) {


		if (i > 0) {

			sf::Vector2f joint = sf::Vector2f(joints[i].x, joints[i].y);
			sf::Vector2f jointPrev = sf::Vector2f(joints[i - 1].x, joints[i - 1].y);

			angleConstraintShape.setPosition(joint);

			sf::Vector2f limbAxis = joint - jointPrev;

			float limbAngle = atan2(limbAxis.y, limbAxis.x);

			angleConstraintShape.setRotation(sf::radians(limbAngle) + sf::degrees(jointAngleConstraints[i].first));
			fabrikEndTexture.draw(angleConstraintShape);

			angleConstraintShape.setRotation(sf::radians(limbAngle) + sf::degrees(jointAngleConstraints[i].second));
			fabrikEndTexture.draw(angleConstraintShape);
		}
		else {
			sf::Vector2f joint = sf::Vector2f(joints[i].x, joints[i].y);

			angleConstraintShape.setPosition(joint);

			angleConstraintShape.setRotation(sf::degrees(jointAngleConstraints[i].first));
			fabrikEndTexture.draw(angleConstraintShape);

			angleConstraintShape.setRotation(sf::degrees(jointAngleConstraints[i].second));
			fabrikEndTexture.draw(angleConstraintShape);
		}
	}

	fabrikEndTexture.display();

	return sf::Texture(fabrikEndTexture.getTexture());
}

int main() {

	srand(TimeHandler::timeRealGet());

	// setup window
	WindowHolder::windowInitialize(sf::VideoMode(sf::Vector2u(1280, 720)), "Window");

	sf::RenderWindow& window = WindowHolder::windowGet();

	Engine::engineInitialize();
	Engine::engineLoad();

	uint32_t frames = 0;
	double frameTimer = 0.0;

	//run main program loop if window is open
	while (WindowHolder::windowGet().isOpen()) {

		window.clear(sf::Color::Black);

		sf::Texture fabrikTexture = fabrikDraw();

		sf::Sprite fabrikSprite(fabrikTexture);

		window.draw(fabrikSprite);

		window.display();


		//TimeHandler::deltaCompute();

		//Engine::engineInputUpdate(WindowHolder::windowGet());
		//Engine::engineUpdate();

		////if (GameData::renderTimer.updateAutoReset(TimeHandler::deltaRealGet())) {
		//	WindowHolder::windowGet().clear(sf::Color::Black);
		//	Engine::engineDraw(WindowHolder::windowGet());
		//	WindowHolder::windowGet().display();
		////}

		//frames++;
		//frameTimer += TimeHandler::deltaRealGet();
		//if (frameTimer > 1.0) {

		//	ConsoleHandler::consolePrintDebug(std::to_string(frames));

		//	frames = 0;
		//	frameTimer = 0.0;
		//}
	}

	Engine::engineSave();
	Engine::engineTerminate();

	return 0;
}