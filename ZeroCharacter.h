#pragma once

#include <string>		// loading path

#include "GameObject.h"


class ZeroCharacter : public GameObject
{
public:
	ZeroCharacter() = default;
	ZeroCharacter(double x, double y, double w, double h, bool isAnimating);

	Definitions::LoadResourcesCommand getLoadResourcesCommand() override;
	void initialize() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;
private:
	double speed;
};