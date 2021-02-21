#include "GameOverScreen.h"
#include "EventsHolder.h"

OBJ_TYPE GameOverScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::GAME_OVER_SCREEN_TYPE;
}

void GameOverScreen::setEvent()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::START_SCREEN_COMMAND);
}
