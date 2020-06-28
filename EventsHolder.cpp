#include "EventsHolder.h"

std::shared_ptr<EventsHolder> EventsHolder::getInstnce()
{
	static std::shared_ptr<EventsHolder> instance{ new EventsHolder };
	return instance;
}

void EventsHolder::initialize()
{
	mode = MODE::MENU_MODE;
	runningGameState = RUN_GAME_STATE::PLAYING_STATE;
	runningMenuState = RUN_MENU_STATE::START_SCREEN_STATE;
	previousRunningMenuState = RUN_MENU_STATE::MAIN_MENU_STATE;
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

void EventsHolder::setEventByButton(BUTTON_TYPE buttonType)
{
	switch (buttonType)
	{
		case BUTTON_TYPE::START_GAME_BUTTON:
			mode = MODE::GAME_MODE;
		break;
		case BUTTON_TYPE::EXIT_GAME_BUTTON:
			mode = MODE::EXIT_MODE;
		break;
		default:
		break;
	}
}

void EventsHolder::setEventByGameCommand(COMMAND command)
{
	switch (command)
	{
		case COMMAND::EXIT_COMMAND:
			mode = MODE::EXIT_MODE;
		break;
		case COMMAND::MENU_COMMAND:
			mode = MODE::MENU_MODE;
		break;
		case COMMAND::GAME_COMMAND:
			mode = MODE::GAME_MODE;
		break;
		case COMMAND::MIAN_MENU_COMMAND:
			runningMenuState = RUN_MENU_STATE::MAIN_MENU_STATE;
		break;
		case COMMAND::FINISHED_LEVEL_SCREEN_COMMAND:
			runningMenuState = RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE;
		break;
		case COMMAND::NEXT_LEVEL_COMMAND:
			mode = MODE::NEXT_LEVEL_MODE;
		break;
		case COMMAND::FINAL_SCREEN_COMMAND:
			runningMenuState = RUN_MENU_STATE::MAIN_MENU_STATE; // todo
		break;
		default:
		break;
	}
}
