#include "StartScreen.h"
#include "EventsHolder.h"

OBJ_TYPE StartScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::START_SCREEN_TYPE;
}

void StartScreen::initialize()
{
}

void StartScreen::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (keysReleased.size() > 0)
	{
		m_keyIsPressed = true;
	}
}

void StartScreen::update()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	if (m_keyIsPressed)
	{
		eventHolder->setEventByGameCommand(Definitions::Command::MIAN_MENU_COMMAND);
		m_keyIsPressed = false;
	}
}