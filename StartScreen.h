#pragma once

#include "PressKeyFullScreen.h"

class StartScreen : public PressKeyFullScreen
{
public:
	StartScreen() : PressKeyFullScreen() {}
	StartScreen(double x, double y, double w, double h, bool animating, double secondsToWaitBeforeAbleToPressKey, std::string backgroundMusicFilename) :
		PressKeyFullScreen(x, y, w, h, animating, secondsToWaitBeforeAbleToPressKey, backgroundMusicFilename)
	{}

	OBJ_TYPE getType() override;

private:
	void setEvent() override;
};