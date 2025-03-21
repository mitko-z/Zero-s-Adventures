#pragma once

#include "MovingCharacter.h"
#include "Health.h"
#include "Timer.h"

class PlayingCharacter : public MovingCharacter
{
public:
	PlayingCharacter(double x, double y, double w, double h, bool isAnimating, double speed, double damage, double health, double attackingSpeed, bool isFlipped = false);

	virtual void takeDamage(double damage);

	virtual double getDamage();

	void update() override;

	void draw(sf::RenderWindow &window) override;

	void loadContent() override;

	virtual bool startAttack();

	virtual void stopAttack();

	bool canMakeNextAttack();

	double getCurrentHealth() { return m_health.currentHealth(); }
	void setCurrentHealth(double currentHealt) { m_health.setCurrentHealth(currentHealt); }

	std::ostringstream getCurrentState() override;

protected:
	virtual void setIsActive();

	virtual void playAttackingSound() {}

private:
	double m_damage;
	double m_attackingSpeed;
	Health m_backgroundHealth, m_health;
	Timer m_attackingTimer;
};