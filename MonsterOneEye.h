#pragma once

#include "Monster.h"

class MonsterOneEye : public Monster
{
public:
	MonsterOneEye() = default;
	MonsterOneEye::MonsterOneEye(double x, double y, double w, double h, double damage, double speed, double health, double attackingSpeed) :
		Monster(x, y, w, h, speed, false, damage, health, attackingSpeed)
	{
	}

	MONSTERS_TYPE getMonsterType() override;
};