#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall(double x, double y, double w, double h, double isAnimating) : GameObject(x, y, w, h, isAnimating) {}

	OBJ_TYPE getType() override;
};