#pragma once
#include "Menu.h"

class SaveMenu : public Menu
{
public:
	SaveMenu() : Menu() {}
	SaveMenu(double x, double y, double w, double h, bool isAnimating, const std::string& backgroundMusicFilename) : Menu(x, y, w, h, isAnimating, backgroundMusicFilename) {}

	void initialize() override;
};