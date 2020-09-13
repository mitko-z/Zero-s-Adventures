#include "Monster.h"

#include "MonsterJelly.h"
#include "MonsterOneEye.h"
#include "MonsterWalkingSquare.h"
#include "ZeroCharacter.h"

#define _USE_MATH_DEFINES
#include <math.h>

OBJ_TYPE Monster::getLoadResourcesCommand()
{
	return OBJ_TYPE::MONSTER_TYPE;
}

MONSTER_TYPES Monster::getMonsterType()
{
	return MONSTER_TYPES::NO_MONSTER_TYPE;
}

void Monster::update()
{

	//// check if Zero can be seen around
	// define and places where the creature to look to
	std::vector<sf::RectangleShape*> placesToLookTo; 
		
	// set the places where to look to - it makes some "circles" around the monster; each next is bigger
	// than the previous one (thanks to the multiplier).
	// it is not clear circles but just it is on its right (when angle is 0 then cos(angle) is 1 and sin(angle) 
	// is 0, so x goes on right, y is the same), on its bottom (angle = PI/2, cos = 0, sin = 1), on its left and top
	for (int i = 10; i < 20; i++)
	{
		for (double angle = 0; angle < 2 * M_PI; angle += 0.5)
		{
			placesToLookTo.push_back(new sf::RectangleShape(sf::Vector2f(m_rect.w, m_rect.h)));
			size_t index = placesToLookTo.size() - 1;
			double multiplier = static_cast<double>(i) / 10.0;
			placesToLookTo[placesToLookTo.size() - 1]->setPosition(
				m_rect.center_x() + 1.5 * m_rect.w * cos(angle) * multiplier,
				m_rect.center_y() + 1.5 * m_rect.h * sin(angle) * multiplier);
		}
	}

	bool foundZero = false;
	extern ResourcesManager *resMan;
	std::vector<GameObject *> gameObjects = resMan->getGameObjects();
	const ZeroCharacter* zeroCharacter = dynamic_cast<ZeroCharacter*>(gameObjects[1]); 
	if (!zeroCharacter) // by def 0th is background, 1st is zero character. But make a check for make sure ;)
	{
		throw "Couldn't find the Zero character at index 1 in the gameObjects list.";
	}
	sf::FloatRect zeroRect(zeroCharacter->getRect().x, zeroCharacter->getRect().y, zeroCharacter->getRect().w, zeroCharacter->getRect().h);

	// look if Zero is around
	for (int i = 0; i < placesToLookTo.size(); i++)
	{
		if(placesToLookTo[i]->getGlobalBounds().intersects(zeroRect))
		{
			foundZero = true;
			break;
		}
	}
	if (foundZero)
	{
		moveTowardsTarget(zeroRect.left, zeroRect.top);
	}
	
	// attack Zero if touching him somehow
	sf::RectangleShape monsterRect(sf::Vector2f(m_rect.w, m_rect.h));
	monsterRect.setPosition(sf::Vector2f(m_rect.x, m_rect.y));
	if (monsterRect.getGlobalBounds().intersects(zeroRect))
	{
		attack();
		m_isAnimating = true;
	}
	else
	{
		stopAttack();
		m_isAnimating = false;
	}

	PlayingCharacter::update();
}

Monster* Monster::createMonster(
	MONSTER_TYPES type, 
	double x, 
	double y, 
	double w, 
	double h, 
	double damage, 
	double speed, 
	double health, 
	double attackingSpeed)
{
	switch (type)
	{
	case MONSTER_TYPES::JELLY_MONSTER_TYPE:
		return new MonsterJelly(x, y, w, h, damage, speed, health, attackingSpeed);
		break;
	case MONSTER_TYPES::ONE_EYE_MONSTER_TYPE:
		return new MonsterOneEye(x, y, w, h, damage, speed, health, attackingSpeed);
		break;
	case MONSTER_TYPES::WALKING_SQUARE_MONSTER_TYPE:
		return new MonsterWalkingSquare(x, y, w, h, damage, speed, health, attackingSpeed);
		break;
	default:
		break;
	}
	return nullptr;
}

void Monster::moveTowardsTarget(int x, int y)
{
	if ((m_rect.x != x) || (m_rect.y != y))
	{
		sf::Vector2i distance;
		distance.x = m_rect.x - x;
		distance.y = m_rect.y - y;
		if (abs(distance.x) > abs(distance.y)) // move towards x
		{
			setDirectionToMove((distance.x < 0) ? MovingDirection::DIRECTION_RIGHT : MovingDirection::DIRECTION_LEFT);
		}
		else
		{
			setDirectionToMove((distance.y < 0) ? MovingDirection::DIRECTION_DOWN : MovingDirection::DIRECTION_UP);
		}
	}
}
