#include "Weapon.h"

#include "WeaponBow.h"

OBJ_TYPE Weapon::getLoadResourcesCommand()
{
	return OBJ_TYPE::WEAPON_TYPE;
}

Weapon * Weapon::createWeapon(WEAPONS_TYPE weaponType, 
							  double x, 
							  double y, 
							  double w, 
							  double h, 
							  bool isAnimating, 
							  double firingRate, 
							  PROJECTILES_TYPE projectilesType, 
							  double projectilesDamage)
{
	switch (weaponType)
	{
	case Definitions::BOW_WEAPON_TYPE:
		return new WeaponBow(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage);
		break;
	case Definitions::FIRE_WEAPON_TYPE:
		break;
	case Definitions::STAR_WEAPON_TYPE:
		break;
	default:
		break;
	}
	return nullptr;
}
