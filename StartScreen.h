#pragma once

#include "Menu.h"

class StartScreen : public Menu
{
public:
	StartScreen() : Menu(), keyIsPressed(false) {}
	StartScreen(double x, double y, double w, double h, bool animating) : Menu(x, y, w, h, animating) {}

	OBJ_TYPE getLoadResourcesCommand() override;
	void initialize() override;
	void update() override;
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
private:
	bool keyIsPressed;		// if a key is pressed -> set event to go to main menu
};