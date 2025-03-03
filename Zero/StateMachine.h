#pragma once

#include <memory>
#include <unordered_map>

#include <SFML\Window.hpp>

#include "Definitions.h"

class StateMachine
{
public:
	static std::shared_ptr<StateMachine> getInstnce();

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
	bool toChangeAudio() { return m_changeAudio; }
	void setToChangeAudio(bool shoudChangeAudio) { m_changeAudio = shoudChangeAudio; }
	void setSaveSlotTo(int saveSlotNumber) { m_saveSlotNumber = saveSlotNumber; }
	int getSaveSlot() { return m_saveSlotNumber; }
private:
	StateMachine();

	UMAP<sf::Keyboard::Key, bool> m_keysPressed;
	UMAP<sf::Keyboard::Key, bool> m_keysReleased;

	// states
	MODE m_mode;
	RUN_GAME_STATE m_runningGameState;
	RUN_MENU_STATE m_runningMenuState;
	RUN_MENU_STATE m_previousRunningMenuState;

	// audio
	bool m_toPlayAudio;
	bool m_changeAudio;

	// Save Slots
	int m_saveSlotNumber;
};