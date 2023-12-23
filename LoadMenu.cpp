#include "LoadMenu.h"

void LoadMenu::initialize()
{
	std::vector<BUTTON_TYPE> buttonTypes;
	for (int i = 0; i < Definitions::saveSlotsNums; i++)
	{
		buttonTypes.push_back(BUTTON_TYPE::LOAD_SLOT);
	}
	buttonTypes.push_back(BUTTON_TYPE::BACK_TO_MAIN_BUTTON);
	setAndInsertButtons(buttonTypes);

	Menu::initialize();
}
