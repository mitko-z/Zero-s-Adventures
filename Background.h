#pragma once
#include "GameObject.h"

class Background : public GameObject
{
public:
	Background() : GameObject(0, 0, 0, 0, false) {}
	Background(double x, double y, double w, double h, bool animating) : GameObject(x, y, w, h, animating) {}

	void loadContent() override;
	Definitions::LoadResourcesCommand getLoadResourcesCommand() override;
};