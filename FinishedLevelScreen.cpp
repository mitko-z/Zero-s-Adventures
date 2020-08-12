#include "FinishedLevelScreen.h"

#include "EventsHolder.h"

OBJ_TYPE FinishedLevelScreen::getLoadResourcesCommand()
{
	return OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE;
}


void FinishedLevelScreen::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (keysReleased.size() > 0)
	{
		keyIsPressed = true;
	}
}

void FinishedLevelScreen::update()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	if (!timer.isStarted())
	{
		timer.start();
	}
	if (timer.elapsedSeconds() > secondsToWaitBeforeNextLevel)
	{
		if (keyIsPressed)
		{
			timer.stop();
			eventHolder->setEventByGameCommand(COMMAND::NEXT_LEVEL_COMMAND);
			keyIsPressed = false;
		}
	}
}