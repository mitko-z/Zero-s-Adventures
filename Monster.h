#pragma once

#include "PlayingCharacter.h"

class Monster : public PlayingCharacter
{
public:
	Monster() = default;
	Monster(
		double x, 
		double y, 
		double w, 
		double h, 
		double speed, 
		bool isAnimating, 
		double damage, 
		double health, 
		double attackingSpeed, 
		const std::vector<OBJ_TYPE>& immuneFromProjectiles) :
		PlayingCharacter(x, y, w, h, isAnimating, speed, damage, health, attackingSpeed),
		m_isTouchingZero(false),
		m_immuneFromProjectiles(immuneFromProjectiles)
	{
	}

	OBJ_TYPE getType() override;

	virtual MONSTERS_TYPE getMonsterType();

	void update() override;

	void playAttackingSound() override;

	static Monster* createMonster(
		MONSTERS_TYPE type, 
		double x, 
		double y, 
		double w, 
		double h, 
		double damage, 
		double speed, 
		double health, 
		double attackingSpeed,
		const std::vector<OBJ_TYPE>& immuneFromProjectiles);

	bool isImmuneFrom(OBJ_TYPE projectileType);
	void setImmuneTo(OBJ_TYPE projectileType) { m_immuneFromProjectiles.push_back(projectileType); }

	std::ostringstream getCurrentState() override;

private:
	void moveTowardsTarget(int x, int y);
	double distanceToTarget(sf::FloatRect target);

	bool m_isTouchingZero;
	std::vector<OBJ_TYPE> m_immuneFromProjectiles;
	
	enum monsterSoundTypes
	{
		MONSTER_SOUND_TYPE_ATTACK = 0,
	};	
};