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
	const UMAP<sf::Keyboard::Key, bool>& getPressedKeys() { return m_keysPressed; }
	void addPressedKey(sf::Keyboard::Key key);
	const UMAP<sf::Keyboard::Key, bool>& getReleasedKeys() { return m_keysReleased; }
	void addReleasedKey(sf::Keyboard::Key key);
	void setEventByButton(BUTTON_TYPE buttonType);
	void setEventByGameCommand(COMMAND command);
	MODE getMode() { return m_mode; }
	RUN_GAME_STATE getRunningGameState() { return m_runningGameState; }
	RUN_MENU_STATE getRunningMenuState() { return m_runningMenuState; }
	RUN_MENU_STATE getPreviousRunningMenuState() { return m_previousRunningMenuState; }
	void nullEvents();
	bool toPlayAudio() { return m_toPlayAudio; }
	void setToPlayAudio(bool ifToPlay) { m_toPlayAudio = ifToPlay; }
private:
	EventsHolder() {}

	UMAP<sf::Keyboard::Key, bool> m_keysPressed;
	UMAP<sf::Keyboard::Key, bool> m_keysReleased;

	// states
	MODE m_mode;
	RUN_GAME_STATE m_runningGameState;
	RUN_MENU_STATE m_runningMenuState;
	RUN_MENU_STATE m_previousRunningMenuState;

	// audio
	bool m_toPlayAudio;
};