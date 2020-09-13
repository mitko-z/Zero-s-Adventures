#pragma once

#include "Monster.h"

class MonsterWalkingSquare : public Monster
{
public:
	MonsterWalkingSquare() = default;
	MonsterWalkingSquare::MonsterWalkingSquare(double x, double y, double w, double h, double damage, double speed, double health, double attackingSpeed) :
		Monster(x, y, w, h, speed, false, damage, health, attackingSpeed)
	{
	}

	MONSTER_TYPES getMonsterType() override;
};