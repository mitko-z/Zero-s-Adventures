#include "SaveMenu.h"

void SaveMenu::initialize()
{
	std::vector<BUTTON_TYPE> buttonTypes;
	for (int i = 0; i < Definitions::saveSlotsNums; i++)
	{
		buttonTypes.push_back(BUTTON_TYPE::SAVE_SLOT);
	}
	buttonTypes.push_back(BUTTON_TYPE::BACK_TO_MAIN_BUTTON);
	setAndInsertButtons(buttonTypes);

	Menu::initialize();
}
