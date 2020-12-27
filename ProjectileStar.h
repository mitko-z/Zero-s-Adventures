#pragma once

#include "Projectile.h"

class ProjectileStar : public Projectile
{
public:
	ProjectileStar() = default;
	ProjectileStar(double x, double y, double w, double h, double speed, double damage, double angleDirection) :
		Projectile(x, y, w, h, speed, damage, angleDirection)
	{}

	OBJ_TYPE getLoadResourcesCommand() override;
};