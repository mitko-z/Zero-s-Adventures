#pragma once

#include "GameObject.h"

class Health : public GameObject
{
public:
	Health(double health, double parentsX, double parentsY, double parentsWidth, double parentsHeight, OBJ_TYPE loadResourcesCommand);

	OBJ_TYPE getLoadResourcesCommand() override;

	void substractHealth(double substractWith);

	void setPosition(double parentsX, double parentsY, double parentsHeight);
private:
	double calculateYPosition(double y, double h);

	OBJ_TYPE m_loadResCommand;
	double m_currentHealth;
	double m_maxHealth;
	double m_maxWidth;
};