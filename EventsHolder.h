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
	void setEventByButton(Definitions::ButtonType buttonType);
	void setEventByGameCommand(Definitions::Command command);
	Definitions::Mode getMode() { return mode; }
	Definitions::RunningGameState getRunningGameState() { return runningGameState; }
	Definitions::RunningMenuState getRunningMenuState() { return runningMenuState; }
	Definitions::RunningMenuState getPreviousRunningMenuState() { return previousRunningMenuState; }
	void nullEvents();
private:
	EventsHolder() {}

	UMAP<sf::Keyboard::Key, bool> keysPressed;
	UMAP<sf::Keyboard::Key, bool> keysReleased;

	// states
	Definitions::Mode mode;
	Definitions::RunningGameState runningGameState;
	Definitions::RunningMenuState runningMenuState;
	Definitions::RunningMenuState previousRunningMenuState;
};