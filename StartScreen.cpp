#include "StartScreen.h"
#include "StateMachine.h"

OBJ_TYPE StartScreen::getType()
{
	return OBJ_TYPE::START_SCREEN_TYPE;
}

void StartScreen::setEvent()
{
	std::shared_ptr<StateMachine> eventHolder = StateMachine::getInstnce();
	eventHolder->setEventByGameCommand(Definitions::Command::MIAN_MENU_COMMAND);
}