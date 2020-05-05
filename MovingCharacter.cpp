#include "MovingCharacter.h"


MovingCharacter::MovingCharacter(double x, double y, double w, double h, bool isAnimating, double speed) :
	GameObject(x, y, w, h, isAnimating),
	speed(speed),
	lastPosition(x, y, w, h)
{
	goOutsideOfScreen = false;
}

void MovingCharacter::update()
{
	lastPosition = rect;

	if (!goOutsideOfScreen)
		setInsideWindow();

	isAnimating = true;
	switch (directionToMove_)
	{
		case MovingDirection::DIRECTION_UP:
			rect.y -= speed;
		break;
		case MovingDirection::DIRECTION_DOWN:
			rect.y += speed;
		break;
		case MovingDirection::DIRECTION_LEFT:
			rect.x -= speed;
		break;
		case MovingDirection::DIRECTION_RIGHT:
			rect.x += speed;
		break;
		default:
			isAnimating = false;
		break;
	}
}

void MovingCharacter::processCollisions()
{
	for (auto colidedObj : objsColideWith)
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
			rect.x = wall.getRect().x - rect.w - 1;
		break;
		case MovingDirection::DIRECTION_LEFT:
			rect.x = wall.getRect().x + wall.getRect().w + 1;
		break;
		case MovingDirection::DIRECTION_DOWN:
			rect.y = wall.getRect().y - rect.h - 1;
		break;
		case MovingDirection::DIRECTION_UP:
			rect.y = wall.getRect().y + wall.getRect().h + 1;
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
	if (rect.x < 0) rect.x = 0;
	if (rect.y < 0) rect.y = 0;
	if ((rect.x + rect.w) > windowW)
	{
		rect.x = windowW - rect.w;
	}
	if ((rect.y + rect.h) > windowH) rect.y = windowH - rect.h;
}