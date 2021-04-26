#include "MainMenu.h"

void MainMenu::initialize()
{
	std::vector<Definitions::ButtonType> buttonTypes
	{
		Definitions::ButtonType::START_GAME_BUTTON,
		Definitions::ButtonType::EXIT_GAME_BUTTON
	};
	setAndInsertButtons(buttonTypes);

	Menu::initialize();
}