#pragma once

#include "Weapon.h"

class WeaponStar : public Weapon
{
public:
	WeaponStar() : Weapon() {}

	WeaponStar(double x, double y, double w, double h, bool isAnimating, double firingRate, PROJECTILES_TYPE projectilesType, double projectilesDamage) :
		Weapon(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage)
	{}
};