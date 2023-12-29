#include "PlayingCharacter.h"


PlayingCharacter::PlayingCharacter(
	double x,
	double y,
	double w,
	double h,
	bool isAnimating,
	double speed,
	double damage,
	double health,
	double attackingSpeed,
	bool isFlipped) :

	MovingCharacter(x, y, w, h, isAnimating, speed, isFlipped),
	m_damage(damage),
	m_backgroundHealth(health, x, y, w, h, OBJ_TYPE::HEALTH_BACKGROUND_TYPE),
	m_health(health, x, y, w, h, OBJ_TYPE::HEALTH_TYPE),
	m_attackingSpeed(attackingSpeed)
{
}

void PlayingCharacter::takeDamage(double damage)
{
	m_health.substractHealth(damage);
}

double PlayingCharacter::getDamage()
{
	return (startAttack() || canMakeNextAttack()) ? m_damage : 0;
}

void PlayingCharacter::update()
{
	// update health
	m_backgroundHealth.setPosition(m_rect.x, m_rect.y, m_rect.h);
	m_health.setPosition(m_rect.x, m_rect.y, m_rect.h);

	m_backgroundHealth.update();
	m_health.update();

	setIsActive();

	// update the rest of itself
	MovingCharacter::update();
}

void PlayingCharacter::draw(sf::RenderWindow & window)
{
	m_backgroundHealth.draw(window);
	m_health.draw(window);
	MovingCharacter::draw(window);
}

void PlayingCharacter::loadContent()
{
	m_backgroundHealth.loadContent();
	m_health.loadContent();
	MovingCharacter::loadContent();
}

bool PlayingCharacter::startAttack()
{
	bool startingNow = false;
	if (!m_attackingTimer.isStarted())
	{
		m_attackingTimer.start();
		startingNow = true;

		playAttackingSound();

		m_isAnimating = true;
	}

	return startingNow;
}

void PlayingCharacter::stopAttack()
{
	if(canMakeNextAttack())
		m_attackingTimer.stop();

	m_isAnimating = false;
}

bool PlayingCharacter::canMakeNextAttack()
{
	bool result(false);
	if (m_attackingTimer.isStarted())
	{
		if (m_attackingTimer.elapsedSeconds() >= m_attackingSpeed)
		{
			m_attackingTimer.restart();
			m_attackingTimer.stop();
			result = true;

			playAttackingSound();

			m_isAnimating = true;
		}
	}
	return result;
}

std::ostringstream PlayingCharacter::getCurrentState()
{
	std::ostringstream oss = MovingCharacter::getCurrentState();
	std::string commentBeginning = "Playing character ";
	oss << addLineForOSS(std::to_string(m_damage), true, commentBeginning + "damage");
	oss << addLineForOSS(std::to_string(m_attackingSpeed), true, commentBeginning + "attacking speed");
	oss << addLineForOSS(std::to_string(m_health.currentHealth()), true, commentBeginning + "current health");
	oss << addLineForOSS(std::to_string(m_health.maxHealth()), true, commentBeginning + "max health");
	return oss;
}

void PlayingCharacter::setIsActive()
{
	if (m_health.currentHealth() <= 0)
	{
		m_isActive = false;
	}
}
