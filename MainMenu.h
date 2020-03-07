#pragma once

#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() : Menu() {}
	MainMenu(double x, double y, double w, double h, bool isAnimating) :
		Menu(x, y, w, h, isAnimating) {}
	void initialize() override;
private:
	
};