#include "MovingCharacter.h"


MovingCharacter::MovingCharacter(double x, double y, double w, double h, bool isAnimating, double speed) :
	GameObject(x, y, w, h, isAnimating),
	m_speed(speed),
	m_lastPosition(x, y, w, h),
	m_goOutsideOfScreen(false),
	m_flipped(false)
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
		switch (colidedObj->getType())
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
	if(this->getType() == OBJ_TYPE::MONSTER_TYPE)
		moveOutsideOfObject(*monster);
}

void MovingCharacter::processZeroCollision(GameObject & zero)
{
	if (this->getType() == OBJ_TYPE::MONSTER_TYPE)
		moveOutsideOfObject(zero);
}

void MovingCharacter::moveOutsideOfObject(GameObject & gameObj)
{
	/*switch (m_directionToMove)
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
	}*/
	// move a little bit of outside of the object
	/*if (m_rect.x == (gameObj.getRect().x + gameObj.getRect().w))
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
	}*/

	Rectangle rect1 = this->m_rect;
	Rectangle rect2 = gameObj.getRect();
	double x1 = rect1.x; double y1 = rect1.y;
	double xw1 = x1 + rect1.w; double yh1 = y1 + rect1.h;
	double x2 = rect2.x; double y2 = rect2.y;
	double xw2 = x2 + rect2.w; double yh2 = y2 + rect2.h;

	if ((xw1 > x2) && (yh1 > y2) && (xw1 < xw2) && (yh1 < yh2)) // rect1 intercects at top left corner rect2
	{ 
		if ((xw1 - x2) < (yh1 - y2)) 
			m_rect.x = rect2.x - m_rect.w - 1;
		else 
			m_rect.y = rect2.y - m_rect.h - 1;
	} 
	else if ((x1 > x2) && (yh1 > y2) && (x1 < xw2) && (yh1 < yh2)) // rect1 intercects at top right corner rect2
	{
		if ((x2 - x1) < (yh1 - y2))
			m_rect.x = xw2 + 1;
		else
			m_rect.y = rect2.y - m_rect.h - 1;
	}
	else if ((xw1 > x2) && (y1 > y2) && (xw1 < xw2) && (y1 < yh2)) // rect1 intercects at bottom left corner rect2
	{
		if ((xw1 - x2) < (y2 - y1))
			m_rect.x = rect2.x - m_rect.w - 1;
		else
			m_rect.y = yh2 + 1;
	}
	else if ((x1 > x2) && (y1 > y2) && (x1 < xw2) && (y1 < yh2)) // rect1 intercects at bottom right corner rect2
	{
		if ((x2 - x1) < (y2 - y1))
			m_rect.x = xw2 + 1;
		else
			m_rect.y = yh2 + 1;
	}
}


void MovingCharacter::setInsideWindow()
{
	double windowW, windowH;
	extern std::shared_ptr <ResourcesManager> resMan;
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
	extern std::shared_ptr <ResourcesManager> resMan;
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

void MovingCharacter::updateDrawingObject()
{
	switch (m_directionToMove)
	{
		case MovingDirection::DIRECTION_LEFT:
			if (!m_flipped)
			{
				int width = m_drawingObject.sprite.getTextureRect().width;
				m_drawingObject.sprite.setOrigin(sf::Vector2f(width, 0));
				m_drawingObject.sprite.scale(-1.f, 1.f);
				m_flipped = true;
			}
		break;
		case MovingDirection::DIRECTION_RIGHT:
			if (m_flipped)
			{
				m_drawingObject.sprite.setOrigin(sf::Vector2f(0, 0));
				m_drawingObject.sprite.scale(-1.f, 1.f);
				m_flipped = false;
			}
		break;
		default:
		break;
	}
	GameObject::updateDrawingObject();
}
