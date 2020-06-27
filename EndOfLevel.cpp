#include "EndOfLevel.h"

#include "EventsHolder.h"

OBJ_TYPE EndOfLevel::getLoadResourcesCommand()
{
	return OBJ_TYPE::END_OF_LEVEL_TYPE;
}

void EndOfLevel::processCollisions()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	for (auto colidedObj : objsColideWith)
	{
		if (colidedObj->getLoadResourcesCommand() == OBJ_TYPE::ZERO_TYPE)
		{
			eventsHolder->setEventByGameCommand(COMMAND::MENU_COMMAND);
			eventsHolder->setEventByGameCommand(COMMAND::FINISHED_LEVEL_SCREEN_COMMAND);
		}
	}
}
