#include "StateMachine.h"

StateMachine::StateMachine()
{
	this->initialize();
}

std::shared_ptr<StateMachine> StateMachine::getInstnce()
{
	static std::shared_ptr<StateMachine> instance{ new StateMachine };
	return instance;
}

void StateMachine::initialize()
{
	m_mode = MODE::MENU_MODE;
	m_runningGameState = RUN_GAME_STATE::PLAYING_STATE;
	m_runningMenuState = RUN_MENU_STATE::START_SCREEN_STATE;
	m_previousRunningMenuState = RUN_MENU_STATE::MAIN_MENU_STATE;

	m_toPlayAudio = true;
	m_changeAudio = true;
}

void StateMachine::addPressedKey(sf::Keyboard::Key key)
{
	m_keysPressed[key] = true;
}

void StateMachine::addReleasedKey(sf::Keyboard::Key key)
{
	m_keysReleased[key] = true;
}

void StateMachine::nullEvents()
{
	m_keysPressed.clear();
	m_keysReleased.clear();
}

void StateMachine::setEventByButton(BUTTON_TYPE buttonType)
{
	switch (buttonType)
	{
		case BUTTON_TYPE::START_GAME_BUTTON:
			m_mode = MODE::GAME_MODE;
			setToChangeAudio(true);
		break;
		case BUTTON_TYPE::EXIT_GAME_BUTTON:
			m_mode = MODE::EXIT_MODE;
		break;
		default:
		break;
	}
}

void StateMachine::setEventByGameCommand(COMMAND command)
{
	setToChangeAudio(true);
	switch (command)
	{
		case COMMAND::EXIT_COMMAND:
			m_mode = MODE::EXIT_MODE;
		break;
		case COMMAND::MENU_COMMAND:
			m_mode = MODE::MENU_MODE;
		break;
		case COMMAND::GAME_COMMAND:
			m_mode = MODE::GAME_MODE;
		break;
		case COMMAND::MIAN_MENU_COMMAND:
			m_runningMenuState = RUN_MENU_STATE::MAIN_MENU_STATE;
		break;
		case COMMAND::FINISHED_LEVEL_SCREEN_COMMAND:
			m_runningMenuState = RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE;
		break;
		case COMMAND::NEXT_LEVEL_COMMAND:
			m_mode = MODE::NEXT_LEVEL_MODE;
		break;
		case COMMAND::FINAL_SCREEN_COMMAND:
			m_runningMenuState = RUN_MENU_STATE::FINAL_SCREEN_STATE;
		break;
		case COMMAND::GAME_OVER_COMMAND:
			m_runningMenuState = RUN_MENU_STATE::GAME_OVER_SCREEN_STATE;
		break;
		case COMMAND::START_SCREEN_COMMAND:
			m_mode = MODE::INITIALIZE_MODE;
		break;
		default:
		break;
	}
}