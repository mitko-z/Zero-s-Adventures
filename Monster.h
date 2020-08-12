#pragma once

#include "MovingCharacter.h"

class Monster : public MovingCharacter
{
public:
	Monster() = default;
	Monster::Monster(double x, double y, double w, double h, double speed, bool isAnimating, double damage) :
		MovingCharacter(x, y, w, h, isAnimating, speed, damage)
	{
	}

	OBJ_TYPE getLoadResourcesCommand() override;

	virtual MONSTER_TYPES getMonsterType();

	void update() override;

	static Monster* createMonster(MONSTER_TYPES type, double x, double y, double w, double h);
};