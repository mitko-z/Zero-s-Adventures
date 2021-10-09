#include "FinalScreen.h"
#include "StateMachine.h"

OBJ_TYPE FinalScreen::getType()
{
	return OBJ_TYPE::FINAL_SCREEN;
}

void FinalScreen::setEvent()
{
	std::shared_ptr<StateMachine> eventHolder = StateMachine::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::START_SCREEN_COMMAND);
}
