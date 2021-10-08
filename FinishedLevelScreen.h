#pragma once

#include "PressKeyFullScreen.h"

class FinishedLevelScreen : public PressKeyFullScreen
{
public:
	FinishedLevelScreen() : PressKeyFullScreen() {}
	FinishedLevelScreen(double x, double y, double w, double h, bool animating, double secondsToWaitBeforeAbleToPressKey, const std::string& backgroundMusicFilename) :
		PressKeyFullScreen(x, y, w, h, animating, secondsToWaitBeforeAbleToPressKey, backgroundMusicFilename) 
	{}

	OBJ_TYPE getType() override;

private:
	void setEvent() override;
};