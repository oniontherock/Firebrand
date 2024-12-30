#ifndef __GAME_STATES_H__
#define __GAME_STATES_H__

#include <Auxiliary/Cooldown.hpp>
#include <GameState/GameStateHandler.hpp>

enum GameStateTypes : uint16_t {
	Play,
	Pause,
	Win,
	Lose,
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

private:
	void gameStateStart();
	void worldClockUpdate();
};



struct GameStatePause : public GameState {

	GameStatePause(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Pause;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;
};

struct GameStateWin : public GameState {

	GameStateWin(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Win;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;
};

struct GameStateLose : public GameState {

	GameStateLose(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Lose;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;

private:
	// cooldown for closing the window
	Cooldown exitCooldown = Cooldown(2.5f);
};



#endif