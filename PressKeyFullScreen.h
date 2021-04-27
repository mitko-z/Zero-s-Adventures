#pragma once

#include "Menu.h"
#include "Timer.h"

class PressKeyFullScreen : public Menu
{
public:
	PressKeyFullScreen() : Menu(), m_keyIsPressed(false) {}
	PressKeyFullScreen(double x, double y, double w, double h, bool animating, double secondsToWaitBeforeNextLevel, std::string backgroundMusicFilename);

	void initialize() override {} // overriding of menu initialization - we do not want any actions here

	virtual OBJ_TYPE getType() = 0;
	void update() override;
	virtual void setEvent() = 0;
	void playBackgroundMusic() override;
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
	void updateEvents() override;
private:
	bool m_keyIsPressed;
	Timer m_timer;
	double m_secondsToWaitBeforeAbleToPressKey = 3.0;
};