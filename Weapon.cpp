#include "Weapon.h"

#include "WeaponBow.h"
#include "WeaponFireball.h"
#include "WeaponStar.h"

Weapon * Weapon::createWeapon(OBJ_TYPE weaponType, 
							  double x, 
							  double y, 
							  double w, 
							  double h, 
							  bool isAnimating, 
							  double firingRate, 
							  OBJ_TYPE projectilesType, 
							  double projectilesDamage,
							  double projectilesSpeed)
{
	switch (weaponType)
	{
	case OBJ_TYPE::BOW_WEAPON_TYPE:
		return new WeaponBow(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage, projectilesSpeed);
		break;
	case OBJ_TYPE::FIREBALL_WEAPON_TYPE:
		return new WeaponFireball(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage, projectilesSpeed);
		break;
	case OBJ_TYPE::STAR_WEAPON_TYPE:
		return new WeaponStar(x, y, w, h, isAnimating, firingRate, projectilesType, projectilesDamage, projectilesSpeed);
		break;
	default:
		break;
	}
	return nullptr;
}

void Weapon::fire(double angle)
{
	extern std::shared_ptr <ResourcesManager> resMan;
	resMan->addGameObject(Projectile::createProjectile(
		m_projectilesType, 
		m_rect.center_x(), 
		m_rect.center_y(), 
		m_rect.w / 2, 
		m_rect.h / 2, 
		m_projectilesDamage, 
		m_projectilesSpeed, 
		angle));
	std::vector<GameObject*> gameObjects = resMan->getGameObjects();
	gameObjects[gameObjects.size() - 1]->loadContent();

	playSound(getType(), weaponSoundTypes::WEAPON_SOUND_TYPE_ATTACK);
}

void Weapon::update()
{
	GameObject::update();
}

void Weapon::draw(sf::RenderWindow & window)
{
	if (!m_isOwned)
	{
		GameObject::draw(window);
	}

	for (auto projectile : m_projectiles)
	{
		projectile->draw(window);
	}
}

std::ostringstream Weapon::getCurrentState()
{
	std::ostringstream oss = GameObject::getCurrentState();
	std::string commentBeginning = "Weapon ";
	oss << addLineForOSS(std::to_string(m_isOwned), true, "if is owned");
	oss << addLineForOSS(std::to_string(m_projectilesType), true, commentBeginning + "type of projectiles it fire");
	oss << addLineForOSS(std::to_string(m_firingRate), true, commentBeginning + "firing rate");
	oss << addLineForOSS(std::to_string(m_projectilesDamage), true, commentBeginning + "projectiles damage");
	oss << addLineForOSS(std::to_string(m_projectilesSpeed), true, commentBeginning + "projectiles speed");
	return oss;
}
