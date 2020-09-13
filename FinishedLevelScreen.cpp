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
		m_keyIsPressed = true;
	}
}

void FinishedLevelScreen::updateEvents()
{
	if (m_timer.isStarted() && (m_timer.elapsedSeconds() > m_secondsToWaitBeforeNextLevel))
	{
		std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
		MAP_KEYS keysPressed = eventsHolder->getPressedKeys();
		MAP_KEYS keysReleased = eventsHolder->getReleasedKeys();
		updateKeys(keysPressed, keysReleased);
	}
}

void FinishedLevelScreen::update()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	if (!m_timer.isStarted())
	{
		m_timer.start();
	}
	if (m_timer.elapsedSeconds() > m_secondsToWaitBeforeNextLevel)
	{
		if (m_keyIsPressed)
		{
			m_timer.stop();
			eventHolder->setEventByGameCommand(COMMAND::NEXT_LEVEL_COMMAND);
			m_keyIsPressed = false;
		}
	}
}