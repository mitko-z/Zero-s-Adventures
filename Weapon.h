#pragma once

#include "GameObject.h"
#include "Projectile.h"

class Weapon : public GameObject
{
public:
	Weapon() = default;
	Weapon(double x, double y, double w, double h, bool isAnimating, double firingRate, OBJ_TYPE projectilesType, double projectilesDamage, double projectilesSpeed) : 
		GameObject(x, y, w, h, isAnimating),
		m_firingRate(firingRate),
		m_projectilesType(projectilesType),
		m_projectilesDamage(projectilesDamage),
		m_projectilesSpeed(projectilesSpeed),
		m_isOwned(false)
	{}

	static Weapon* createWeapon(OBJ_TYPE weaponType,
								double x, 
								double y, 
								double w, 
								double h, 
								bool isAnimating, 
								double firingRate, 
								OBJ_TYPE projectilesType, 
								double projectilesDamage,
								double projectilesSpeed);

	void setIsOwned(bool isOwned) { m_isOwned = isOwned; }
	
	bool isOwned() { return m_isOwned; }

	void fire(double angle);

	void update() override;

	void draw(sf::RenderWindow &window) override;

	sf::Sprite getWeaponSprite() { return m_drawingObject.sprite; }

private:
	OBJ_TYPE m_projectilesType;
	double m_firingRate;
	double m_projectilesDamage;
	double m_projectilesSpeed;
	bool m_isOwned;
	std::vector<Projectile*> m_projectiles;
	enum weaponSoundTypes
	{
		WEAPON_SOUND_TYPE_ATTACK = 0,
	};
};