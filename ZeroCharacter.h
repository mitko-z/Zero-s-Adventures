#pragma once

#include <string>		// loading path

#include "PlayingCharacter.h"
#include "Weapon.h"

class ZeroCharacter : public PlayingCharacter
{
public:
	ZeroCharacter() = default;
	ZeroCharacter(double x, double y, double w, double h, double speed, double health);

	OBJ_TYPE getLoadResourcesCommand() override;
	void initialize() override;
	void update() override;
	void processCollisions() override;
	void attack();
	void calculateFiringAngle(double& angle);
	Weapon* getCurrentWeapon() { return m_weapon; }
	void setWeapon(Weapon* weapon) { m_weapon = weapon; }
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;

private:
	bool m_takeWeapon;
	Weapon* m_weapon;
	MovingDirection m_lastDirection;
};