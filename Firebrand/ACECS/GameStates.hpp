#ifndef __GAME_STATES_H__
#define __GAME_STATES_H__

#include <GameState/GameStateHandler.hpp>

enum GameStateTypes : uint16_t {
	Play,
	Pause,
};

struct GameStatePlay : public GameState {

	GameStatePlay(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Play;
	}


	enum Modes {
		Normal,
		Edit,
	};

	Modes mode = Normal;

	bool isFirstRun = true;

	void gameStateUpdate() final;
};


struct GameStatePause : public GameState {

	GameStatePause(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Pause;
	}


	enum Modes {
		Normal,
		Xray,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;
};



#endif