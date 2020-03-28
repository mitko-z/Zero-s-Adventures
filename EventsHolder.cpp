#include "EventsHolder.h"

std::shared_ptr<EventsHolder> EventsHolder::getInstnce()
{
	static std::shared_ptr<EventsHolder> instance{ new EventsHolder };
	return instance;
}

void EventsHolder::initialize()
{
	mode = Definitions::Mode::menuMode;
	runningGameState = Definitions::RunningGameState::StartScreenState;
	runningMenuState = Definitions::RunningMenuState::MainMenuState;
	previousRunningMenuState = Definitions::RunningMenuState::MainMenuState;
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
		case Definitions::StartGame:
			mode = Definitions::Mode::gameMode;
		break;
		case Definitions::ExitGame:
			mode = Definitions::Mode::exitMode;
		break;
		default:
		break;
	}
}

void EventsHolder::setEventByGameCommand(Definitions::GameCommand command)
{
	switch (command)
	{
		case Definitions::GameCommand::ExitCommand:
			mode = Definitions::Mode::exitMode;
		break;
		case Definitions::GameCommand::MainMenuCommand:
			mode = Definitions::Mode::menuMode;
		break;
		case Definitions::GameCommand::GameModeCommand:
			mode = Definitions::Mode::gameMode;
		break;
		default:
		break;
	}
}
