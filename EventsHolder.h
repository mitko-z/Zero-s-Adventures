#pragma once

#include <memory>
#include <unordered_map>

#include <SFML\Window.hpp>

#include "Definitions.h"

class EventsHolder
{
public:
	static std::shared_ptr<EventsHolder> getInstnce();

	void initialize();
	const UMAP<sf::Keyboard::Key, bool>& getPressedKeys() { return keysPressed; }
	void addPressedKey(sf::Keyboard::Key key);
	const UMAP<sf::Keyboard::Key, bool>& getReleasedKeys() { return keysReleased; }
	void addReleasedKey(sf::Keyboard::Key key);
	void setEventByButton(BUTTON_TYPE buttonType);
	void setEventByGameCommand(COMMAND command);
	MODE getMode() { return mode; }
	RUN_GAME_STATE getRunningGameState() { return runningGameState; }
	RUN_MENU_STATE getRunningMenuState() { return runningMenuState; }
	RUN_MENU_STATE getPreviousRunningMenuState() { return previousRunningMenuState; }
	void nullEvents();
private:
	EventsHolder() {}

	UMAP<sf::Keyboard::Key, bool> keysPressed;
	UMAP<sf::Keyboard::Key, bool> keysReleased;

	// states
	MODE mode;
	RUN_GAME_STATE runningGameState;
	RUN_MENU_STATE runningMenuState;
	RUN_MENU_STATE previousRunningMenuState;
};