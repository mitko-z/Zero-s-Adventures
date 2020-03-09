#include "MainMenu.h"

void MainMenu::initialize()
{
	std::vector<Definitions::ButtonType> buttonTypes
	{
		Definitions::ButtonType::StartGame,
		Definitions::ButtonType::ExitGame
	};
	setAndInsertButtons(buttonTypes);
	Menu::initialize();
}