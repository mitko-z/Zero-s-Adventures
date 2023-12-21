#pragma once

#include "Menu.h"

class ResumeGameMenu : public Menu
{
public:
	ResumeGameMenu() : Menu() {}
	ResumeGameMenu(double x, double y, double w, double h, bool isAnimating, const std::string& backgroundMusicFilename) :
		Menu(x, y, w, h, isAnimating, backgroundMusicFilename) {}
	void initialize() override;
private:

};