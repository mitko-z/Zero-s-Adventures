#pragma once

#include "MovingCharacter.h"

class Projectile : public MovingCharacter
{
public:
	Projectile() = default;
	Projectile(double x, double y, double w, double h, double speed, double damage, double angleDirection);

	static Projectile* createProjectile(OBJ_TYPE type, double x, double y, double w, double h, double damage, double speed, double angle);

	void updateDirectionToMove() override;

	void processWallCollision(GameObject& wall) override;
	void processMonsterCollision(GameObject* monster) override;

private:
	double m_angleDirection;
	double m_damage;
};