#include "StartScreen.h"
#include "EventsHolder.h"

void StartScreen::loadContent()
{
	GameObject::loadContent();
	scaleSpriteTo(rect.w, rect.h, drawingObject.texture, drawingObject.sprite);
}

LOAD_RES_COMMAND StartScreen::getLoadResourcesCommand()
{
	return LOAD_RES_COMMAND::START_SCREEN_RES;
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
		eventHolder->setEventByGameCommand(Definitions::GameCommand::MIAN_MENU_COMMAND);
		keyIsPressed = false;
	}
}