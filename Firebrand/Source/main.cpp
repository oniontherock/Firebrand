#include "../ACECS/ACECS.hpp"
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

int main() {

	TimeHandler::deltaCompute();

	srand(TimeHandler::timeRealGet());

	// setup window
	WindowHolder::windowInitialize(sf::VideoMode(1280, 720), "Window");
	WindowHolder::windowGet().setFramerateLimit(60);

	Engine::engineInitialize();
	//Engine::engineLoad();

	//uint32_t frames = 0;
	//double frameTimer = 0.0;

	CollisionShape shapeA(CollisionPolygon{ sf::Vector2f(-64, -64), sf::Vector2f(64, -64), sf::Vector2f(64, 64), sf::Vector2f(-64, 64) });
	shapeA.centerSet(sf::Vector2f(640.f, 360.f));
	CollisionShape shapeB(CollisionPolygon{ sf::Vector2f(-32, 0), sf::Vector2f(64, -128), sf::Vector2f(128, 0), sf::Vector2f(12, 128) });
	shapeB.centerSet(sf::Vector2f(640.f - 320.f, 360.f));

	//run main program loop if window is open
	while (WindowHolder::windowGet().isOpen()) {

		WindowHolder::windowGet().clear();

		sf::ConvexShape convexShapeA;
		convexShapeA.setPointCount(shapeA.vertices.size());
		for (uint16_t i = 0; i < shapeA.vertices.size(); i++) {
			convexShapeA.setPoint(i, shapeA.vertices[i]);
		}

		sf::ConvexShape convexShapeB;
		convexShapeB.setPointCount(shapeB.vertices.size());
		for (uint16_t i = 0; i < shapeB.vertices.size(); i++) {
			convexShapeB.setPoint(i, shapeB.vertices[i]);
		}

		convexShapeA.setFillColor(sf::Color::Blue);
		convexShapeB.setFillColor(sf::Color::Red);

		WindowHolder::windowGet().draw(convexShapeA);
		WindowHolder::windowGet().draw(convexShapeB);

		WindowHolder::windowGet().display();


		//TimeHandler::deltaCompute();

		Engine::engineInputUpdate(WindowHolder::windowGet());

		sf::Vector2f inputAxis;
		inputAxis.x = float(InputInterface::inputGetActive("Move Right") - InputInterface::inputGetActive("Move Left"));
		inputAxis.y = float(InputInterface::inputGetActive("Move Down") - InputInterface::inputGetActive("Move Up"));

		sf::Vector2f moveAxis = Vector2fMath::normalize(inputAxis) * 120.f * static_cast<float>(TimeHandler::deltaSimulatedGet());

		if (InputInterface::inputGetActive("Rotate Left")) {
			shapeA.rotationAdd(-Mathf::PI / 128);
		}
		if (InputInterface::inputGetActive("Rotate Right")) {
			shapeA.rotationAdd(Mathf::PI / 128);
		}
		shapeB.rotationAdd(Mathf::PI / 256);

		if (inputAxis.x != 0 || inputAxis.y != 0) {

			shapeA.centerAdd(moveAxis);
		}

		if (CollisionHandler::collisionCheck(shapeA, shapeB)) {
			shapeA.centerAdd(-CollisionHandler::collisionVectorGet(shapeA, shapeB));
		}
		//Engine::engineUpdate();

		//WindowHolder::windowGet().clear(sf::Color::Black);
		//Engine::engineDraw(WindowHolder::windowGet());
		//WindowHolder::windowGet().display();

		//frames++;
		//frameTimer += TimeHandler::deltaRealGet();
		//if (frameTimer > 1.0) {

		//	ConsoleHandler::consolePrintDebug(std::to_string(frames));

		//	frames = 0;
		//	frameTimer = 0.0;
		//}
	}

	//Engine::engineSave();
	Engine::engineTerminate();

	return 0;
}