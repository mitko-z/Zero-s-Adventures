#pragma once

#include "Monster.h"

#define DAMAGE 7
#define SPEED 7
#define IS_ANIMATING false

class MonsterOneEye : public Monster
{
public:
	MonsterOneEye() = default;
	MonsterOneEye::MonsterOneEye(double x, double y, double w, double h) :
		Monster(x, y, w, h, IS_ANIMATING, SPEED, DAMAGE)
	{
	}

	MONSTER_TYPES getMonsterType() override;
};