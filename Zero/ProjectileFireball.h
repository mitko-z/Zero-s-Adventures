#pragma once

#include "Projectile.h"

class ProjectileFireball : public Projectile
{
public:
	ProjectileFireball() = default;
	ProjectileFireball(double x, double y, double w, double h, double speed, double damage, double angleDirection) :
		Projectile(x, y, w, h, speed, damage, angleDirection)
	{}

	OBJ_TYPE getType() override;
};