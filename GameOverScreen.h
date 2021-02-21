#pragma once

#include "PressKeyFullScreen.h"

class GameOverScreen : public PressKeyFullScreen
{
public:
	GameOverScreen() : PressKeyFullScreen() {}
	GameOverScreen(double x, double y, double w, double h, bool animating, double secondsToWaitBeforeAbleToPressKey) : 
		PressKeyFullScreen(x, y, w, h, animating, secondsToWaitBeforeAbleToPressKey) 
	{}

	OBJ_TYPE getLoadResourcesCommand() override;

private:
	void setEvent() override;
};