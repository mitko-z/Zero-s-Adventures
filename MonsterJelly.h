#pragma once

#include "Monster.h"

#define DAMAGE 5
#define SPEED 5
#define IS_ANIMATING false

class MonsterJelly : public Monster
{
public:
	MonsterJelly() = default;
	MonsterJelly::MonsterJelly(double x, double y, double w, double h) :
		Monster(x, y, w, h, IS_ANIMATING, SPEED, DAMAGE)
	{
	}

	MONSTER_TYPES getMonsterType() override;
};