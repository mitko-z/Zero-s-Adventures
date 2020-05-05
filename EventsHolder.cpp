#include "EventsHolder.h"

std::shared_ptr<EventsHolder> EventsHolder::getInstnce()
{
	static std::shared_ptr<EventsHolder> instance{ new EventsHolder };
	return instance;
}

void EventsHolder::initialize()
{
	mode = Definitions::Mode::MENU_MODE;
	runningGameState = Definitions::RunningGameState::PLAYING_STATE;
	runningMenuState = Definitions::RunningMenuState::START_SCREEN_STATE;
	previousRunningMenuState = Definitions::RunningMenuState::MAIN_MENU_STATE;
}

void EventsHolder::addPressedKey(sf::Keyboard::Key key)
{
	keysPressed[key] = true;
}

void EventsHolder::addReleasedKey(sf::Keyboard::Key key)
{
	keysReleased[key] = true;
}

void EventsHolder::nullEvents()
{
	keysPressed.clear();
	keysReleased.clear();
}

void EventsHolder::setEventByButton(Definitions::ButtonType buttonType)
{
	switch (buttonType)
	{
		case Definitions::START_GAME_BUTTON:
			mode = Definitions::Mode::GAME_MODE;
		break;
		case Definitions::EXIT_GAME_BUTTON:
			mode = Definitions::Mode::EXIT_MODE;
		break;
		default:
		break;
	}
}

void EventsHolder::setEventByGameCommand(Definitions::Command command)
{
	switch (command)
	{
		case Definitions::Command::EXIT_COMMAND:
			mode = Definitions::Mode::EXIT_MODE;
		break;
		case Definitions::Command::MENU_COMMAND:
			mode = Definitions::Mode::MENU_MODE;
		break;
		case Definitions::Command::GAME_COMMAND:
			mode = Definitions::Mode::GAME_MODE;
		break;
		case Definitions::Command::MIAN_MENU_COMMAND:
			runningMenuState = Definitions::RunningMenuState::MAIN_MENU_STATE;
		break;
		default:
		break;
	}
}
