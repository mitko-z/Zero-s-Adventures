#include "Monster.h"

#include "MonsterJelly.h"
#include "MonsterOneEye.h"
#include "MonsterWalkingSquare.h"
#include "ZeroCharacter.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_DISTANCE_TO_LOOK_FOR_ZERO 3 // 3 blocks

OBJ_TYPE Monster::getType()
{
	return OBJ_TYPE::MONSTER_TYPE;
}

MONSTERS_TYPE Monster::getMonsterType()
{
	return MONSTERS_TYPE::NO_MONSTER_TYPE;
}

void Monster::update()
{

	//// check if Zero can be seen around
	// define and places where the creature to look to
	std::vector<sf::RectangleShape> placesToLookTo; 

	// look if Zero is around
	bool foundZero = false;
	extern ResourcesManager *resMan;
	std::vector<GameObject *> gameObjects = resMan->getGameObjects();
	const ZeroCharacter* zeroCharacter = dynamic_cast<ZeroCharacter*>(gameObjects[1]); 
	if (!zeroCharacter) // by def 0th is background, 1st is zero character. But make a check for make sure ;)
	{
		throw "Couldn't find the Zero character at index 1 in the gameObjects list.";
	}
	sf::FloatRect zeroRect(zeroCharacter->getRect().x, zeroCharacter->getRect().y, zeroCharacter->getRect().w, zeroCharacter->getRect().h);
	double distanceInPixels = MAX_DISTANCE_TO_LOOK_FOR_ZERO * (resMan->getLevelBlockDimensions().x + resMan->getLevelBlockDimensions().y) / 2;
	if (distanceToTarget(zeroRect) < distanceInPixels)
	{
		moveTowardsTarget(zeroRect.left, zeroRect.top);
	}
	
	// attack Zero if touching him somehow
	sf::RectangleShape monsterRect(sf::Vector2f(m_rect.w, m_rect.h));
	monsterRect.setPosition(sf::Vector2f(m_rect.x, m_rect.y));
	if (monsterRect.getGlobalBounds().intersects(zeroRect))
	{
		startAttack();
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
	MONSTERS_TYPE type, 
	double x, 
	double y, 
	double w, 
	double h, 
	double damage, 
	double speed, 
	double health, 
	double attackingSpeed,
	const std::vector<OBJ_TYPE>& immuneFromProjectiles)
{
	switch (type)
	{
	case MONSTERS_TYPE::JELLY_MONSTER_TYPE:
		return new MonsterJelly(x, y, w, h, damage, speed, health, attackingSpeed, immuneFromProjectiles);
		break;
	case MONSTERS_TYPE::ONE_EYE_MONSTER_TYPE:
		return new MonsterOneEye(x, y, w, h, damage, speed, health, attackingSpeed, immuneFromProjectiles);
		break;
	case MONSTERS_TYPE::WALKING_SQUARE_MONSTER_TYPE:
		return new MonsterWalkingSquare(x, y, w, h, damage, speed, health, attackingSpeed, immuneFromProjectiles);
		break;
	default:
		break;
	}
	return nullptr;
}

bool Monster::isImmuneFrom(OBJ_TYPE projectileType)
{
	bool isImmune = false;
	for (auto& type : m_immuneFromProjectiles)
	{
		if (projectileType == type)
		{
			isImmune = true;
			break;
		}
	}
	return isImmune;
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

double Monster::distanceToTarget(sf::FloatRect target)
{
	sf::Vector2f targetCenter(target.left + target.width / 2, target.top + target.height / 2);
	return sqrt((m_rect.x - targetCenter.x)*(m_rect.x - targetCenter.x) + (m_rect.y - targetCenter.y)*(m_rect.y - targetCenter.y));
}
