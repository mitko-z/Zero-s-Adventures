#include "Projectile.h"

#include "ProjectileArrow.h"
#include "ProjectileFireball.h"
#include "ProjectileStar.h"
#include "Monster.h"

#define _USE_MATH_DEFINES
#include <math.h>

Projectile::Projectile(double x, double y, double w, double h, double speed, double damage, double angleDirection) :
	MovingCharacter(x, y, w, h, false, speed),
	m_angleDirection(angleDirection),
	m_damage(damage)
{
	m_goOutsideOfScreen = true;
	this->m_drawingObject.sprite.rotate(angleDirection + 180);
}

Projectile * Projectile::createProjectile(OBJ_TYPE type, double x, double y, double w, double h, double damage, double speed, double angle)
{
	switch (type)
	{
	case Definitions::ARROW_PROJECTILE_TYPE:
		return new ProjectileArrow(x, y, w, h, speed, damage, angle);
		break;
	case Definitions::FIREBALL_PROJECTILE_TYPE:
		return new ProjectileFireball(x, y, w, h, speed, damage, angle);
		break;
	case Definitions::STAR_PROJECTILE_TYPE:
		return new ProjectileStar(x, y, w, h, speed, damage, angle);
		break;
	default:
		break;
	}
	return nullptr;
}

void Projectile::update()
{
	if (isOutsideOfScreen())
		m_isActive = false;
	MovingCharacter::update();
}

void Projectile::updateDirectionToMove()
{
	m_isAnimating = true; // projectiles always move
	m_rect.x += cos(m_angleDirection * M_PI / 180);
	m_rect.y += sin(m_angleDirection * M_PI / 180);
}

void Projectile::processWallCollision(GameObject & wall)
{
	m_isActive = false;
}

void Projectile::processMonsterCollision(GameObject* monster)
{
	dynamic_cast<Monster*>(monster)->takeDamage(m_damage);
	m_isActive = false;
}

bool Projectile::isOutsideOfScreen()
{
	double windowW, windowH;
	extern ResourcesManager *resMan;
	resMan->getWindowDimensions(windowW, windowH);
	if (m_rect.x < 0 ||
		m_rect.y < 0 ||
		(m_rect.x + m_rect.w) > windowW ||
		(m_rect.y + m_rect.h) > windowH)
		return true;
	return false;
}
