#include "EndOfLevel.h"

#include "EventsHolder.h"

OBJ_TYPE EndOfLevel::getLoadResourcesCommand()
{
	return OBJ_TYPE::END_OF_LEVEL_TYPE;
}

void EndOfLevel::processCollisions()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	for (auto colidedObj : m_objsColideWith)
	{
		if (colidedObj->getLoadResourcesCommand() == OBJ_TYPE::ZERO_TYPE)
		{
			eventsHolder->setEventByGameCommand(COMMAND::MENU_COMMAND);
			COMMAND setScreenCommand = m_isLastLevel ? 
										COMMAND::FINAL_SCREEN_COMMAND : 
										COMMAND::FINISHED_LEVEL_SCREEN_COMMAND;
			eventsHolder->setEventByGameCommand(setScreenCommand);
		}
	}
}
