#include "MovingCharacter.h"


MovingCharacter::MovingCharacter(double x, double y, double w, double h, bool isAnimating, double speed, double damage) :
	GameObject(x, y, w, h, isAnimating),
	speed(speed),
	lastPosition(x, y, w, h),
	health(MAX_HEALTH),
	damage(damage),
	goOutsideOfScreen(false)
{}

void MovingCharacter::update()
{
	lastPosition = m_rect;

	if (!goOutsideOfScreen)
		setInsideWindow();

	m_isAnimating = true;
	switch (directionToMove_)
	{
		case MovingDirection::DIRECTION_UP:
			m_rect.y -= speed;
		break;
		case MovingDirection::DIRECTION_DOWN:
			m_rect.y += speed;
		break;
		case MovingDirection::DIRECTION_LEFT:
			m_rect.x -= speed;
		break;
		case MovingDirection::DIRECTION_RIGHT:
			m_rect.x += speed;
		break;
		default:
			m_isAnimating = false;
		break;
	}
}

void MovingCharacter::processCollisions()
{
	for (auto colidedObj : m_objsColideWith)
	{
		switch (colidedObj->getLoadResourcesCommand())
		{
			case OBJ_TYPE::WALL_TYPE:
				processWallCollision(*colidedObj);
			break;
			default:
			break;
		}
	}

	setDirectionToMove(MovingDirection::DIRECTION_NONE);
}

void MovingCharacter::processWallCollision(GameObject & wall)
{
	switch (directionToMove_)
	{
		case MovingDirection::DIRECTION_RIGHT:
			m_rect.x = wall.getRect().x - m_rect.w - 1;
		break;
		case MovingDirection::DIRECTION_LEFT:
			m_rect.x = wall.getRect().x + wall.getRect().w + 1;
		break;
		case MovingDirection::DIRECTION_DOWN:
			m_rect.y = wall.getRect().y - m_rect.h - 1;
		break;
		case MovingDirection::DIRECTION_UP:
			m_rect.y = wall.getRect().y + wall.getRect().h + 1;
		break;
		default:
		break;
	}
}


void MovingCharacter::setInsideWindow()
{
	float windowW, windowH;
	extern ResourcesManager *resMan;
	resMan->getWindowDimensions(windowW, windowH);
	if (m_rect.x < 0) m_rect.x = 0;
	if (m_rect.y < 0) m_rect.y = 0;
	if ((m_rect.x + m_rect.w) > windowW)
	{
		m_rect.x = windowW - m_rect.w;
	}
	if ((m_rect.y + m_rect.h) > windowH) m_rect.y = windowH - m_rect.h;
}