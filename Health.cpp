#include "Health.h"

#define HEALTH_HEIGHT_FACTOR 15
#define HEALTH_Y_OFSET_FACTOR 15
//#define CALCULATE_Y(y, height) (y - ((height) / (HEALTH_Y_OFSET_FACTOR)))

Health::Health(double health, double parentsX, double parentsY, double parentsWidth, double parentsHeight, OBJ_TYPE loadResourcesCommand) :
	// GameObject(parentsX, CALCULATE_Y(parentsY, parentsHeight), parentsWidth, parentsHeight / HEALTH_HEIGHT_FACTOR, false)
	GameObject(parentsX, calculateYPosition(parentsY, parentsHeight), parentsWidth, parentsHeight / HEALTH_HEIGHT_FACTOR, false),
	m_currentHealth(health),
	m_maxHealth(health),
	m_maxWidth(parentsWidth),
	m_loadResCommand(loadResourcesCommand)
{
}

OBJ_TYPE Health::getLoadResourcesCommand()
{
	return m_loadResCommand;
}

void Health::substractHealth(double substractWith)
{
	m_currentHealth -= substractWith;
	if (m_currentHealth < 0.0)
		m_currentHealth = 0;
	double newWidth = m_maxWidth * (m_currentHealth / m_maxHealth);
	scaleSpriteTo(newWidth, m_rect.h, m_drawingObject.texture, m_drawingObject.sprite);
}

void Health::setPosition(double parentsX, double parentsY, double parentsHeight)
{
	m_rect.x = parentsX;
	m_rect.y = calculateYPosition(parentsY, parentsHeight);
	updateDrawingObject();
}

double Health::calculateYPosition(double y, double h)
{
	return (y - h / HEALTH_Y_OFSET_FACTOR);
}
