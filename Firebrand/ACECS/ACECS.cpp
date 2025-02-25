#include "../Include/Game/AI/Teams/TeamHolder.hpp"
#include "../Include/Game/World/Procedural Generation/Structure Generation/Prop Types/PropTypeRegistry.hpp"
#include "../Include/Game/World/Procedural Generation/Structure Generation/Room Types/RoomTypeRegistry.hpp"
#include "ACECS.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "GameStates.hpp"
#include "Panels.hpp"
#include "RNGPoolIdRegistry.hpp"
#include <Audio.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <GameState.hpp>
#include <Graphics.hpp>
#include <Input.hpp>
#include <Saving/SaveDirector.hpp>
#include <World.hpp>

void Engine::inputsRegister() {
	InputInterface::inputRegister("Pause", KeySet{ KeyEvent("Escape", Pressed) });

	InputInterface::inputRegister("Move Right", KeySet{ KeyEvent("D", Held), KeyEvent("Right", Held) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Move Up", KeySet{ KeyEvent("W", Held), KeyEvent("Up", Held) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Move Left", KeySet{ KeyEvent("A", Held), KeyEvent("Left", Held) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Move Down", KeySet{ KeyEvent("S", Held), KeyEvent("Down", Held) }, InputKeyLogic::Or);

	InputInterface::inputRegister("Toggle ObjectGrid Rendering", KeySet{ KeyEvent("Numpad1", Pressed) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Toggle Paths Rendering", KeySet{ KeyEvent("Numpad2", Pressed) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Toggle Timescale", KeySet{ KeyEvent("Numpad3", Pressed) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Toggle Collision Shapes Draw", KeySet{ KeyEvent("Numpad4", Pressed) }, InputKeyLogic::Or);
}

// game states are registered here
void Engine::panelsRegister() {
	using namespace PanelManager;
	panelAdd(PanelTypes::StaticView, PanelPtr(new PanelStaticView(
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // screen coordinates
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // world coordinates
		sf::Color::Black
	)));
	panelAdd(PanelTypes::DynamicView, PanelPtr(new PanelDynamicView(
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // screen coordinates
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // world coordinates
		sf::Color::Transparent
	)));
	panelAdd(PanelTypes::Hud, PanelPtr(new PanelHud(
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // screen coordinates
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // world coordinates
		sf::Color::Transparent
	)));
	panelAdd(PanelTypes::WinScreen, PanelPtr(new PanelWinScreen(
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // screen coordinates
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // world coordinates
		sf::Color::Black
	)));
	panelAdd(PanelTypes::LoseScreen, PanelPtr(new PanelLoseScreen(
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // screen coordinates
		PanelRect(sf::Vector2f(0, 0), sf::Vector2f(1280, 720)), // world coordinates
		sf::Color::Black
	)));
}

// game states are registered here
void Engine::gameStatesRegister() {

	GameStateHandler::gameStateAdd(GameStateUniquePtr(new GameStatePlay(
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition(GameStateType::Pause, // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Pause",
				}
			),
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			PanelTypes::StaticView,
			PanelTypes::DynamicView,
			PanelTypes::Hud,
		}
		)));

	GameStateHandler::gameStateAdd(GameStateUniquePtr(new GameStatePause(
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition(GameStateType::Play, // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Pause",
				}
			),
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			PanelTypes::StaticView,
			PanelTypes::DynamicView,
			PanelTypes::Hud,
		}
		)));

	GameStateHandler::gameStateAdd(GameStateUniquePtr(new GameStateWin(
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			PanelTypes::WinScreen,
		}
		)));
	GameStateHandler::gameStateAdd(GameStateUniquePtr(new GameStateLose(
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			PanelTypes::LoseScreen,
		}
		)));

	GameStateHandler::gameStatesAddedStatesFinalize();
	GameStateHandler::gameStateForceSet(GameStateTypes::Play);
}
void Engine::audioRegister() {
	// register human footsteps
	for (uint16_t i = 1; i <= 8; i++) {

		sf::Sound soundCur(AudioStore::soundBufferStore.fileGetOrLoadFromName("SFX/Footstep" + std::to_string(i)));

		soundCur.setMinDistance(256);
		soundCur.setAttenuation(12);
		soundCur.setVolume(25);

		SoundHandler::soundAdd("HumanFootstep", soundCur);
	}
}
void Engine::imagesRegister() {
	GraphicsStore::imageStore.fileLoadFromName("Art/Circle");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Floors/Floor Plank");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Floors/Floor Tile");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Walls/Wall Wooden Single");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Walls/Wall Wooden Straight");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Walls/Wall Wooden Corner");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Walls/Wall Wooden Junction T");
	GraphicsStore::imageStore.fileLoadFromName("Art/Structures/Walls/Wall Wooden Junction Plus");
}
void Engine::texturesRegister() {
	GraphicsStore::textureStore.objectAddFromInstance("Circle", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Circle")));
	GraphicsStore::textureStore.objectAddFromInstance("Floor Plank", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Floors/Floor Plank")));
	GraphicsStore::textureStore.objectAddFromInstance("Floor Tile", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Floors/Floor Tile")));
	GraphicsStore::textureStore.objectAddFromInstance("Wall Wooden Single", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Walls/Wall Wooden Single")));
	GraphicsStore::textureStore.objectAddFromInstance("Wall Wooden Straight", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Walls/Wall Wooden Straight")));
	GraphicsStore::textureStore.objectAddFromInstance("Wall Wooden Corner", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Walls/Wall Wooden Corner")));
	GraphicsStore::textureStore.objectAddFromInstance("Wall Wooden Junction T", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Walls/Wall Wooden Junction T")));
	GraphicsStore::textureStore.objectAddFromInstance("Wall Wooden Junction Plus", sf::Texture(GraphicsStore::imageStore.objectGet("Art/Structures/Walls/Wall Wooden Junction Plus")));
}
void Engine::RNGPoolsRegister() {

	RNGf::initialize();

	RNGfPool::poolCreate(10000);
	RNGfPool::poolFillRange(PROBABILITY_POOL_ID, 0.f, 1.f);

	RNGu16Pool::poolCreate(UINT16_MAX);
	RNGu16Pool::poolFillRange(UINT_POOL_ID, 0, UINT16_MAX);

	RNGi16Pool::poolCreate(32);
	RNGi16Pool::poolFillRange(INT_SIGN_POOL_ID, -1, 2);
}

// initialize the ACECS engine by registering all inputs, initializing the ECS module, and registering game states.
// of course, certain modules do not have to be initialized if the user does not want them to be
void Engine::engineInitialize() {

	RNGPoolsRegister();

	RoomTypeRegistry::roomTypesRegister();
	PropTypeRegistry::propTypesRegister();

	Teams::TeamHolder::initialize();

	GameLevelGrid::levelGridInitialize(1, 1, 1);
	GameLevelGrid::levelAdd(new GameLevel(0, 0, 0), true);

	// register textures and images
	imagesRegister();
	texturesRegister();

	inputsRegister();
	panelsRegister();
	ECSRegistry::ECSInitialize();
	audioRegister();
	gameStatesRegister();
}
// updates the engines input
void Engine::engineInputUpdate(sf::RenderWindow& window) {
	InputInterface::inputUpdate();

	InputInterface::eventsProcess(window);
}
// update certain modules of the engine, like the game state.
// note that certain modules, like the ECS system, are updated inside the GameStateHandler,
// because you don't want to update the ECS system if the GameState is currently paused, for example.
void Engine::engineUpdate() {
	GameStateHandler::gameStateProcess();
}
void Engine::engineDraw(sf::RenderWindow& renderWindowMain) {

	auto& gameStatePanels = GameStateHandler::gameStateGetPanels();

	for (uint16_t i = 0; i < gameStatePanels.size(); i++) {
		PanelManager::panelGet(gameStatePanels[i]).panelDrawObjects();
		PanelManager::panelGet(gameStatePanels[i]).panelRender(renderWindowMain);
		PanelManager::panelGet(gameStatePanels[i]).panelClear();
	}
}
// terminates certain engine modules, like the ECS or GameStateHandler
void Engine::engineTerminate() {
	ECSRegistry::ECSTerminate();
	GameStateHandler::gameStatesAllTerminate();
	PanelManager::panelManagerTerminate();
}

void Engine::engineSave() {
	SaveDirector::saveBegin();
	SaveDirector::gameDataSave();
	SaveDirector::saveEnd();
}
void Engine::engineLoad() {
	// only load if a save file exists
	if (SaveHandler::saveFileExists()) {
		SaveDirector::loadBegin();
		SaveDirector::gameDataLoad();
		SaveDirector::loadEnd();
	}
}
