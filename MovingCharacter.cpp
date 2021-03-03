#include "MovingCharacter.h"


MovingCharacter::MovingCharacter(double x, double y, double w, double h, bool isAnimating, double speed) :
	GameObject(x, y, w, h, isAnimating),
	m_speed(speed),
	m_lastPosition(x, y, w, h),
	m_goOutsideOfScreen(false)
{}

void MovingCharacter::update()
{
	m_lastPosition = m_rect;

	if (!m_goOutsideOfScreen)
		setInsideWindow();
	updateDirectionToMove();

	updateDrawingObject();
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
			case OBJ_TYPE::MONSTER_TYPE:
				processMonsterCollision(colidedObj);
			break;
			case OBJ_TYPE::ZERO_TYPE:
				processZeroCollision(*colidedObj);
			break;
			default:
			break;
		}
	}

	setDirectionToMove(MovingDirection::DIRECTION_NONE);
}

void MovingCharacter::processWallCollision(GameObject & wall)
{
	moveOutsideOfObject(wall);
}

void MovingCharacter::processMonsterCollision(GameObject* monster)
{
	if(this->getLoadResourcesCommand() == OBJ_TYPE::MONSTER_TYPE)
		moveOutsideOfObject(*monster);
}

void MovingCharacter::processZeroCollision(GameObject & zero)
{
	if (this->getLoadResourcesCommand() == OBJ_TYPE::MONSTER_TYPE)
		moveOutsideOfObject(zero);
}

void MovingCharacter::moveOutsideOfObject(GameObject & gameObj)
{
	switch (m_directionToMove)
	{
	case MovingDirection::DIRECTION_RIGHT:
		m_rect.x = gameObj.getRect().x - m_rect.w - 1;
		break;
	case MovingDirection::DIRECTION_LEFT:
		m_rect.x = gameObj.getRect().x + gameObj.getRect().w + 1;
		break;
	case MovingDirection::DIRECTION_DOWN:
		m_rect.y = gameObj.getRect().y - m_rect.h - 1;
		break;
	case MovingDirection::DIRECTION_UP:
		m_rect.y = gameObj.getRect().y + gameObj.getRect().h + 1;
		break;
	default:
		break;
	}
	// move a little bit of outside of the wall
	if (m_rect.x == (gameObj.getRect().x + gameObj.getRect().w))
	{
		m_rect.x++;
	}
	else if ((m_rect.x + m_rect.w) == gameObj.getRect().x)
	{
		m_rect.x--;
	}
	if (m_rect.y == (gameObj.getRect().y + gameObj.getRect().h))
	{
		m_rect.y++;
	}
	else if ((m_rect.y + m_rect.h) == gameObj.getRect().y)
	{
		m_rect.y--;
	}
}


void MovingCharacter::setInsideWindow()
{
	double windowW, windowH;
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

void MovingCharacter::updateDirectionToMove()
{
	m_isAnimating = true;
	extern ResourcesManager *resMan;
	switch (m_directionToMove)
	{
	case MovingDirection::DIRECTION_UP:
		m_rect.y -= m_speed * resMan->getSpeedFactor().y;
		break;
	case MovingDirection::DIRECTION_DOWN:
		m_rect.y += m_speed * resMan->getSpeedFactor().y;
		break;
	case MovingDirection::DIRECTION_LEFT:
		m_rect.x -= m_speed * resMan->getSpeedFactor().x;
		break;
	case MovingDirection::DIRECTION_RIGHT:
		m_rect.x += m_speed * resMan->getSpeedFactor().x;
		break;
	default:
		m_isAnimating = false;
		break;
	}
}
