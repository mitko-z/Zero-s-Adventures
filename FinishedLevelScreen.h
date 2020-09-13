#pragma once

#include "Menu.h"
#include "Timer.h"

class FinishedLevelScreen : public Menu
{
public:
	FinishedLevelScreen() : Menu(), m_keyIsPressed(false) {}
	FinishedLevelScreen(double x, double y, double w, double h, bool animating) : Menu(x, y, w, h, animating), m_keyIsPressed(false) {}

	OBJ_TYPE getLoadResourcesCommand() override;
	void initialize() override {}
	void update() override;
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
	void updateEvents() override;
private:
	bool m_keyIsPressed;
	Timer m_timer;
	const double m_secondsToWaitBeforeNextLevel = 3.0;
};