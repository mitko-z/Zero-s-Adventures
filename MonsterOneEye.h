#pragma once

#include "Monster.h"

#define IS_ANIMATING false

class MonsterOneEye : public Monster
{
public:
	MonsterOneEye() = default;
	MonsterOneEye::MonsterOneEye(double x, double y, double w, double h, double damage, double speed) :
		Monster(x, y, w, h, speed, IS_ANIMATING, damage)
	{
	}

	MONSTER_TYPES getMonsterType() override;
};