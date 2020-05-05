#pragma once

#include <string>		// loading path

#include "MovingCharacter.h"


class ZeroCharacter : public MovingCharacter
{
public:
	ZeroCharacter() = default;
	ZeroCharacter(double x, double y, double w, double h, double speed, bool isAnimating);

	Definitions::ObjectType getLoadResourcesCommand() override;
	void initialize() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
};