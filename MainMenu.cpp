#include "MainMenu.h"

void MainMenu::initialize()
{
	std::vector<BUTTON_TYPE> buttonTypes
	{
		BUTTON_TYPE::START_GAME_BUTTON,
		BUTTON_TYPE::LOAD_GAME_BUTTON,
		BUTTON_TYPE::EXIT_GAME_BUTTON
	};
	setAndInsertButtons(buttonTypes);

	Menu::initialize();
}