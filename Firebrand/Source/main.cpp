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
#include "../Include/Game/GameData.hpp"
#include <Saving/SaveHandler.hpp>
#include <SFML/Window.hpp>


int main() {

	TimeHandler::deltaCompute();

	srand(TimeHandler::timeRealGet());

	// setup window
	WindowHolder::windowInitialize(sf::VideoMode(1280, 720), "Window");
	//WindowHolder::windowGet().setFramerateLimit(60);

	Engine::engineInitialize();
	Engine::engineLoad();

	uint32_t frames = 0;
	double frameTimer = 0.0;

	//run main program loop if window is open
	while (WindowHolder::windowGet().isOpen()) {

		TimeHandler::deltaCompute();

		Engine::engineInputUpdate(WindowHolder::windowGet());
		Engine::engineUpdate();

		//if (GameData::renderTimer.updateAutoReset(TimeHandler::deltaRealGet())) {
			WindowHolder::windowGet().clear(sf::Color::Black);
			Engine::engineDraw(WindowHolder::windowGet());
			WindowHolder::windowGet().display();
		//}

		frames++;
		frameTimer += TimeHandler::deltaRealGet();
		if (frameTimer > 1.0) {

			ConsoleHandler::consolePrintDebug(std::to_string(frames));

			frames = 0;
			frameTimer = 0.0;
		}
	}

	Engine::engineSave();
	Engine::engineTerminate();

	return 0;
}