#include "FinishedLevelScreen.h"

#include "EventsHolder.h"

OBJ_TYPE FinishedLevelScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE;
}

void FinishedLevelScreen::setEvent()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::NEXT_LEVEL_COMMAND);
}
