#pragma once

#include "GameObject.h"
#include "Wall.h"

#define MAX_HEALTH 100

class MovingCharacter : public GameObject
{
public:
	MovingCharacter(double x, double y, double w, double h, bool isAnimating, double speed);
	void update() override;
	virtual void processCollisions();
protected:
	enum MovingDirection
	{
		DIRECTION_NONE,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT
	};
	void setDirectionToMove(MovingDirection direction) { directionToMove_ = direction; }
	MovingDirection directionToMove() { return directionToMove_; }
	void processWallCollision(GameObject& wall);
	Rectangle lastPosition;
	double speed;
	bool goOutsideOfScreen;
private:
	void setInsideWindow();
	MovingDirection directionToMove_;
};