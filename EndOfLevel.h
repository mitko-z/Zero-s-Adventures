#pragma once


#include "GameObject.h"

class EndOfLevel : public GameObject
{
public:
	EndOfLevel(double x, double y, double w, double h, double isAnimating) : GameObject(x, y, w, h, isAnimating) {}

	OBJ_TYPE getLoadResourcesCommand() override;
	void processCollisions() override;
};