#include "FinishedLevelScreen.h"

#include "EventsHolder.h"

OBJ_TYPE FinishedLevelScreen::getType()
{
	return OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE;
}

void FinishedLevelScreen::setEvent()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::NEXT_LEVEL_COMMAND);
}
