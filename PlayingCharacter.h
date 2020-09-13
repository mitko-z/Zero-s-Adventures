#pragma once

#include "MovingCharacter.h"
#include "Health.h"
#include "Timer.h"

class PlayingCharacter : public MovingCharacter
{
public:
	PlayingCharacter(double x, double y, double w, double h, bool isAnimating, double speed, double damage, double health, double attackingSpeed);

	virtual void takeDamage(double damage);

	virtual double getDamage();

	void update() override;

	void draw(sf::RenderWindow &window) override;

	void loadContent() override;

	void attack();

	void stopAttack();

private:

	bool canMakeNextAttack();

	double m_damage;
	double m_attackingSpeed;
	Health m_health;
	Timer m_attackingTimer;
};