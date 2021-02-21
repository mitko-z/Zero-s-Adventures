#include "PressKeyFullScreen.h"
#include "EventsHolder.h"

PressKeyFullScreen::PressKeyFullScreen(double x, double y, double w, double h, bool animating, double secondsToWaitBeforeAbleToPressKey) :
	Menu(x, y, w, h, animating),
	m_keyIsPressed(false),
	m_secondsToWaitBeforeAbleToPressKey(secondsToWaitBeforeAbleToPressKey)
{}

void PressKeyFullScreen::update()
{
	if (!m_timer.isStarted())
	{
		m_timer.start();
	}
	if (m_timer.elapsedSeconds() > m_secondsToWaitBeforeAbleToPressKey)
	{
		if (m_keyIsPressed)
		{
			m_timer.stop();
			m_keyIsPressed = false;
			setEvent();
		}
	}
}

void PressKeyFullScreen::updateKeys(const MAP_KEYS & keysPressed, const MAP_KEYS & keysReleased)
{
	if (keysReleased.size() > 0)
	{
		m_keyIsPressed = true;
	}
}

void PressKeyFullScreen::updateEvents()
{
	if (m_timer.isStarted() && (m_timer.elapsedSeconds() > m_secondsToWaitBeforeAbleToPressKey))
	{
		std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
		MAP_KEYS keysPressed = eventsHolder->getPressedKeys();
		MAP_KEYS keysReleased = eventsHolder->getReleasedKeys();
		updateKeys(keysPressed, keysReleased);
	}
}
