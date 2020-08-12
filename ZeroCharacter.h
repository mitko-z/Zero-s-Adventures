#pragma once

#include <string>		// loading path

#include "MovingCharacter.h"

#define ZERO_SPEED 10
#define IS_ANIMATING false

class ZeroCharacter : public MovingCharacter
{
public:
	ZeroCharacter() = default;
	ZeroCharacter(double x, double y, double w, double h);

	OBJ_TYPE getLoadResourcesCommand() override;
	void initialize() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;
protected:
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
};