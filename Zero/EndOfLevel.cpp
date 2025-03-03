#include "EndOfLevel.h"

#include "StateMachine.h"

OBJ_TYPE EndOfLevel::getType()
{
	return OBJ_TYPE::END_OF_LEVEL_TYPE;
}

void EndOfLevel::processCollisions()
{
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	for (auto colidedObj : m_objsColideWith)
	{
		if (colidedObj->getType() == OBJ_TYPE::ZERO_TYPE)
		{
			stateMachine->setEventByGameCommand(COMMAND::MENU_COMMAND);
			COMMAND setScreenCommand = m_isLastLevel ? 
										COMMAND::FINAL_SCREEN_COMMAND : 
										COMMAND::FINISHED_LEVEL_SCREEN_COMMAND;
			stateMachine->setEventByGameCommand(setScreenCommand);
		}
	}
}

std::ostringstream EndOfLevel::getCurrentState()
{
	std::ostringstream oss = GameObject::getCurrentState();
	oss << addLineForOSS(std::to_string(m_isLastLevel), true, "End of level if it is the last level");

	return oss;
}
