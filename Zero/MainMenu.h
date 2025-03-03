#pragma once

#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() : Menu() {}
	MainMenu(double x, double y, double w, double h, bool isAnimating, const std::string& backgroundMusicFilename) :
		Menu(x, y, w, h, isAnimating, backgroundMusicFilename) {}
	void initialize() override;
private:
	
};