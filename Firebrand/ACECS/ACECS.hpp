#ifndef __ACECS_H__
#define __ACECS_H__

#include <Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "SaveOperations.hpp"

#define GAME_STATE_FUNCTION []()
#define PANEL_DRAW_FUNCTION [](Panel& panel)

class Engine {
private:
	// inputs are registered here
	static void inputsRegister();
	// game states are registered here
	static void panelsRegister();
	// game states are registered here
	static void gameStatesRegister();
	// audio is registered here
	static void audioRegister();
	// registers all sorts of image files
	static void imagesRegister();
	// registers all sorts of textures
	static void texturesRegister();
public:
	// saves the engine
	static void engineSave();
	// loads the engine, doesn't load if save file doesn't exist
	static void engineLoad();
	// initialize the ACECS engine by registering all inputs, initializing the ECS module, and registering game states.
	// of course, certain modules do not have to be initialized if the user does not want them to be
	static void engineInitialize();
	// updates the engines input
	static void engineInputUpdate(sf::RenderWindow& window);
	// update certain modules of the engine, like the input system, and the game state.
	// note that certain modules, like the ECS system, are updated inside the GameStateHandler,
	// because you don't want to update the ECS system if the GameState is currently paused, for example.
	static void engineUpdate();
	static void engineDraw(sf::RenderWindow& renderWindowMain);
	// terminates certain engine modules, like the ECS or GameStateHandler
	static void engineTerminate();
};


#endif