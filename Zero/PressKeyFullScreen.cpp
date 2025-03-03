#include "PressKeyFullScreen.h"
#include "BackgroundAudioPlayer.h"
#include "StateMachine.h"

PressKeyFullScreen::PressKeyFullScreen(
								double x, 
								double y, 
								double w, 
								double h, 
								bool animating, 
								double secondsToWaitBeforeAbleToPressKey, 
								const std::string& backgroundMusicFilename) :
	Menu(x, y, w, h, animating, backgroundMusicFilename),
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

void PressKeyFullScreen::playBackgroundMusic()
{
	BackgroundAudioPlayer::getInstance()->play(m_backgroundMusicFilename, false);
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
		std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
		MAP_KEYS keysPressed = stateMachine->getPressedKeys();
		MAP_KEYS keysReleased = stateMachine->getReleasedKeys();
		updateKeys(keysPressed, keysReleased);
	}
}
