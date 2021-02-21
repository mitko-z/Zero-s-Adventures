#include "StartScreen.h"
#include "EventsHolder.h"

OBJ_TYPE StartScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::START_SCREEN_TYPE;
}

void StartScreen::setEvent()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	eventHolder->setEventByGameCommand(Definitions::Command::MIAN_MENU_COMMAND);
}