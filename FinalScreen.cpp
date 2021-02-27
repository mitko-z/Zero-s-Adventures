#include "FinalScreen.h"
#include "EventsHolder.h"

OBJ_TYPE FinalScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::FINAL_SCREEN;
}

void FinalScreen::setEvent()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::START_SCREEN_COMMAND);
}
