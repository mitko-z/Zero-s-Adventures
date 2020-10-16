#include "Weapon.h"

#include "WeaponBow.h"
#include "WeaponFireball.h"
#include "WeaponStar.h"

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
	case WEAPONS_TYPE::BOW_WEAPON_TYPE:
		return new WeaponBow(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage);
		break;
	case WEAPONS_TYPE::FIREBALL_WEAPON_TYPE:
		return new WeaponFireball(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage);
		break;
	case WEAPONS_TYPE::STAR_WEAPON_TYPE:
		return new WeaponStar(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage);
		break;
	default:
		break;
	}
	return nullptr;
}

void Weapon::draw(sf::RenderWindow & window)
{
	if (!m_isOwned)
	{
		GameObject::draw(window);
	}
}
