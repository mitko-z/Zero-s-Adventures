#pragma once

#include "Monster.h"

#define DAMAGE 15
#define SPEED 15
#define IS_ANIMATING false

class MonsterWalkingSquare : public Monster
{
public:
	MonsterWalkingSquare() = default;
	MonsterWalkingSquare::MonsterWalkingSquare(double x, double y, double w, double h) :
		Monster(x, y, w, h, IS_ANIMATING, SPEED, DAMAGE)
	{
	}

	MONSTER_TYPES getMonsterType() override;
};