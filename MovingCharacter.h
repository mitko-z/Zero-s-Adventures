#pragma once

#include "GameObject.h"
#include "Wall.h"

#define MAX_HEALTH 100

class MovingCharacter : public GameObject
{
public:
	MovingCharacter(double x, double y, double w, double h, bool isAnimating, double speed);
	virtual void update();
	virtual void processCollisions();

protected:
	enum MovingDirection
	{
		DIRECTION_NONE = -1,
		DIRECTION_UP = 270,
		DIRECTION_DOWN = 90,
		DIRECTION_LEFT = 180,
		DIRECTION_RIGHT = 0
	};
	void setDirectionToMove(MovingDirection direction) { m_directionToMove = direction; }
	MovingDirection directionToMove() { return m_directionToMove; }
	virtual void processWallCollision(GameObject& wall);
	virtual void processMonsterCollision(GameObject* monster);
	void setInsideWindow();
	virtual void updateDirectionToMove();
	
	Rectangle m_lastPosition;
	double m_speed;
	bool m_goOutsideOfScreen;
	MovingDirection m_directionToMove;
};