#pragma once

#include "Menu.h"

class LoadMenu : public Menu
{
public:
	LoadMenu() : Menu() {}
	LoadMenu(double x, double y, double w, double h, bool isAnimating, const std::string& backgroundMusicFilename) : Menu(x, y, w, h, isAnimating, backgroundMusicFilename) {}

	void initialize() override;
};