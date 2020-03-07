#include "MainMenu.h"

void MainMenu::initialize()
{
	std::vector<std::string> texts
	{
		"START GAME",
		"EXIT GAME"
	};
	setAndInsertButtons(texts);
	// highlighter.setRect(Rectangle(10, 10, 10, 10));
	Menu::initialize();
}