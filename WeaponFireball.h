#pragma once

#include "Weapon.h"

class WeaponFireball : public Weapon
{
public:
	WeaponFireball() : Weapon() {}

	WeaponFireball(double x, double y, double w, double h, bool isAnimating, double firingRate, PROJECTILES_TYPE projectilesType, double projectilesDamage) :
		Weapon(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage)
	{}
};