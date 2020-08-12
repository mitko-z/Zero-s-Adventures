#include "Monster.h"

#include "MonsterJelly.h"
#include "MonsterOneEye.h"
#include "MonsterWalkingSquare.h"

OBJ_TYPE Monster::getLoadResourcesCommand()
{
	return OBJ_TYPE::MONSTER_TYPE;
}

MONSTER_TYPES Monster::getMonsterType()
{
	return MONSTER_TYPES::NO_MONSTER_TYPE;
}

void Monster::update()
{
	// todo: set movement towards zero when in range
	MovingCharacter::update();
	updateDrawingObject();
}

Monster* Monster::createMonster(MONSTER_TYPES type, double x, double y, double w, double h)
{
	switch (type)
	{
	case MONSTER_TYPES::JELLY_MONSTER_TYPE:
		return new MonsterJelly(x, y, w, h);
		break;
	case MONSTER_TYPES::ONE_EYE_MONSTER_TYPE:
		return new MonsterOneEye(x, y, w, h);
		break;
	case MONSTER_TYPES::WALKING_SQUARE_MONSTER_TYPE:
		return new MonsterWalkingSquare(x, y, w, h);
		break;
	default:
		break;
	}
	return nullptr;
}
