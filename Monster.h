#pragma once

#include "PlayingCharacter.h"

class Monster : public PlayingCharacter
{
public:
	Monster() = default;
	Monster::Monster(double x, double y, double w, double h, double speed, bool isAnimating, double damage, double health, double attackingSpeed) :
		PlayingCharacter(x, y, w, h, isAnimating, speed, damage, health, attackingSpeed),
		m_isTouchingZero(false)
	{
	}

	OBJ_TYPE getLoadResourcesCommand() override;

	virtual MONSTER_TYPES getMonsterType();

	void update() override;

	static Monster* createMonster(MONSTER_TYPES type, double x, double y, double w, double h, double damage, double speed, double health, double attackingSpeed);

private:
	void moveTowardsTarget(int x, int y);

	bool m_isTouchingZero;
};