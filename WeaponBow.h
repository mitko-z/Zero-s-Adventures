#pragma once

#include "Weapon.h"

class WeaponBow : public Weapon
{
public:
	WeaponBow() : Weapon() {}

	WeaponBow(double x, double y, double w, double h, bool isAnimating, double firingRate, OBJ_TYPE projectilesType, double projectilesDamage, double projectilesSpeed) :
		Weapon(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage, projectilesSpeed)
	{}

	OBJ_TYPE getLoadResourcesCommand() override;
};