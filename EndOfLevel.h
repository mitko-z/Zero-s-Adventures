#pragma once


#include "GameObject.h"

class EndOfLevel : public GameObject
{
public:
	EndOfLevel(double x, double y, double w, double h, double isAnimating, bool isLastLevel) : 
		GameObject(x, y, w, h, isAnimating),
		m_isLastLevel(isLastLevel)
	{}

	OBJ_TYPE getType() override;
	void processCollisions() override;

private:
	bool m_isLastLevel;
};