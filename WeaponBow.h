#pragma once

#include "Weapon.h"

class WeaponBow : public Weapon
{
public:
	WeaponBow() : Weapon() {}

	WeaponBow(double x, double y, double w, double h, bool isAnimating, double firingRate, PROJECTILES_TYPE projectilesType, double projectilesDamage) :
		Weapon(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage)
	{}
};