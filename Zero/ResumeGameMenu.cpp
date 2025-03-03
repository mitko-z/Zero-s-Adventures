#include "ResumeGameMenu.h"

void ResumeGameMenu::initialize()
{
	std::vector<BUTTON_TYPE> buttonTypes
	{
		BUTTON_TYPE::RESUME_GAME_BUTTON,
		BUTTON_TYPE::LOAD_GAME_BUTTON,
		BUTTON_TYPE::SAVE_GAME_BUTTON,
		BUTTON_TYPE::EXIT_GAME_BUTTON
	};
	setAndInsertButtons(buttonTypes);

	Menu::initialize();
}
