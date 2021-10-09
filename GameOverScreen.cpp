#include "GameOverScreen.h"
#include "StateMachine.h"

OBJ_TYPE GameOverScreen::getType()
{
	return OBJ_TYPE::GAME_OVER_SCREEN_TYPE;
}

void GameOverScreen::setEvent()
{
	std::shared_ptr<StateMachine> eventHolder = StateMachine::getInstnce();
	eventHolder->setEventByGameCommand(COMMAND::START_SCREEN_COMMAND);
}
