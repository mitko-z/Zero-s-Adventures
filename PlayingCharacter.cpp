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
	double attackingSpeed) :

	MovingCharacter(x, y, w, h, isAnimating, speed),
	m_damage(damage),
	m_health(health, x, y, w, h),
	m_attackingSpeed(attackingSpeed)
{
}

void PlayingCharacter::takeDamage(double damage)
{
	m_health.substractHealth(damage);
}

double PlayingCharacter::getDamage()
{
	return canMakeNextAttack() ? m_damage : 0;
}

void PlayingCharacter::update()
{
	// update health
	m_health.setPosition(m_rect.x, m_rect.y, m_rect.h);
	m_health.update();

	// update the rest of itself
	MovingCharacter::update();
}

void PlayingCharacter::draw(sf::RenderWindow & window)
{
	m_health.draw(window);
	MovingCharacter::draw(window);
}

void PlayingCharacter::loadContent()
{
	m_health.loadContent();
	MovingCharacter::loadContent();
}

void PlayingCharacter::attack()
{
	if (!m_attackingTimer.isStarted())
		m_attackingTimer.start();
}

void PlayingCharacter::stopAttack()
{
	if(m_attackingTimer.isStarted())
		m_attackingTimer.stop();
}

bool PlayingCharacter::canMakeNextAttack()
{
	bool result(false);
	if (m_attackingTimer.isStarted())
	{
		if (m_attackingTimer.elapsedSeconds() >= m_attackingSpeed)
		{
			m_attackingTimer.restart();
			result = true;
		}
	}
	return result;
}
