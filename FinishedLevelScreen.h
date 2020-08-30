#pragma once

#include "Menu.h"
#include "Timer.h"

class FinishedLevelScreen : public Menu
{
public:
	FinishedLevelScreen() : Menu(), keyIsPressed(false) {}
	FinishedLevelScreen(double x, double y, double w, double h, bool animating) : Menu(x, y, w, h, animating), keyIsPressed(false) {}

	OBJ_TYPE getLoadResourcesCommand() override;
	void initialize() override {}
	void update() override;
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
	void updateEvents() override;
private:
	bool keyIsPressed;
	Timer timer;
	const double secondsToWaitBeforeNextLevel = 3.0;
};