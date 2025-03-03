#pragma once

#include "GameObject.h"
#include "Definitions.h"

class ButtonHighlighter : public GameObject
{
public:
	ButtonHighlighter() : GameObject(0, 0, 0, 0, false) {}
	ButtonHighlighter(
		double x,
		double y,
		double w,
		double h,
		bool isAnimating) :
		GameObject(x, y, w, h, isAnimating)
	{}

	Definitions::ObjectType getType() override;
};