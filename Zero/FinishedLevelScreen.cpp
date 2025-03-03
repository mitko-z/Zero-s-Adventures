#include "FinishedLevelScreen.h"

#include "StateMachine.h"

OBJ_TYPE FinishedLevelScreen::getType()
{
	return OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE;
}

void FinishedLevelScreen::setEvent()
{
	std::shared_ptr<StateMachine> eventHolder = StateMachine::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::NEXT_LEVEL_COMMAND);
}
