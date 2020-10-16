#pragma once

#include "GameObject.h"

class Weapon : public GameObject
{
public:
	Weapon() : GameObject(0, 0, 0, 0, false) {}
	Weapon(double x, double y, double w, double h, bool isAnimating, double firingRate, PROJECTILES_TYPE projectilesType, double projectilesDamage) : 
		GameObject(x, y, w, h, isAnimating),
		m_firingRate(firingRate),
		m_projectilesType(projectilesType),
		m_projectilesDamage(projectilesDamage)
	{}

	OBJ_TYPE getLoadResourcesCommand() override;

	static Weapon* createWeapon(WEAPONS_TYPE weaponType,
								double x, 
								double y, 
								double w, 
								double h, 
								bool isAnimating, 
								double firingRate, 
								PROJECTILES_TYPE projectilesType, 
								double projectilesDamage);

	void setIsOwned(bool isOwned) { m_isOwned = isOwned; }
	bool isOwned() { return m_isOwned; }

	void draw(sf::RenderWindow &window) override;
private:
	PROJECTILES_TYPE m_projectilesType;
	double m_firingRate;
	double m_projectilesDamage;
	bool m_isOwned;
};