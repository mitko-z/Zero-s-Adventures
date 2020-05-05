#include "StartScreen.h"
#include "EventsHolder.h"

OBJ_TYPE StartScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::START_SCREEN_TYPE;
}

void StartScreen::initialize()
{
	GameObject::initialize();
}

void StartScreen::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (keysReleased.size() > 0)
	{
		keyIsPressed = true;
	}
}

void StartScreen::update()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	if (keyIsPressed)
	{
		eventHolder->setEventByGameCommand(Definitions::Command::MIAN_MENU_COMMAND);
		keyIsPressed = false;
	}
}