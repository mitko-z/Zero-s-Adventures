#pragma once

#include "Projectile.h"

class ProjectileArrow : public Projectile
{
public:
	ProjectileArrow() = default;
	ProjectileArrow(double x, double y, double w, double h, double speed, double damage, double angleDirection) :
		Projectile(x, y, w, h, speed, damage, angleDirection) 
	{}

	OBJ_TYPE getLoadResourcesCommand() override;
};