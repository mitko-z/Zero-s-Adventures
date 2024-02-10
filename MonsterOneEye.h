#pragma once

#include "Monster.h"

class MonsterOneEye : public Monster
{
public:
	MonsterOneEye() = default;
	MonsterOneEye(double x, double y, double w, double h, double damage, double speed, double health, double attackingSpeed, const std::vector<OBJ_TYPE>& immuneFromProjectiles) :
		Monster(x, y, w, h, speed, false, damage, health, attackingSpeed, immuneFromProjectiles)
	{
	}

	MONSTERS_TYPE getMonsterType() override;
};