#pragma once

#include "PressKeyFullScreen.h"

class FinalScreen : public PressKeyFullScreen
{
public:
	FinalScreen(double x, double y, double w, double h, bool animating, double secondsToWaitBeforeAbleToPressKey) :
		PressKeyFullScreen(x, y, w, h, animating, secondsToWaitBeforeAbleToPressKey) 
	{}

	OBJ_TYPE getLoadResourcesCommand() override;

	void setEvent() override;
};