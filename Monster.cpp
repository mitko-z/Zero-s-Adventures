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
	
	// placesToLookTo[0].getGlobalBounds().intersects
	
	// set the places where to look to - it makes two "circles" around the creature; the second is twice bigger
	// than the first one (thanks to the multiplier).
	// it is not clear circles but just it is on its right (when angle is 0 then cos(angle) is 1 and sin(angle) 
	// is 0, so x goes on right, y is the same), on its bottom (angle = PI/2, cos = 0, sin = 1), on its left and top
	for (int i = 10; i < 20; i += 5)
	{
		for (double angle = 0; angle < 2 * M_PI; angle += 0.5)
		{
			placesToLookTo.push_back(new sf::RectangleShape(sf::Vector2f(m_rect.w, m_rect.h)));
			size_t index = placesToLookTo.size() - 1;
			/*placesToLookTo[index]->setOrigin(
				m_rect.center_x() + 1.5 * m_rect.w * cos(angle) * multiplier,
				m_rect.center_y() + 1.5 * m_rect.h * sin(angle) * multiplier);*/
			double multiplier = static_cast<double>(i) / 10.0;
			placesToLookTo[placesToLookTo.size() - 1]->setPosition(
				m_rect.center_x() + 1.5 * m_rect.w * cos(angle) * multiplier,
				m_rect.center_y() + 1.5 * m_rect.h * sin(angle) * multiplier);
			/*(
				(int)(destinationRectangle.Center.X + 1.5 * destinationRectangle.Width * Math.Cos(angle) * multiplier),
				(int)(destinationRectangle.Center.Y + 1.5 * destinationRectangle.Height * Math.Sin(angle) * multiplier),
				destinationRectangle.Width,
				destinationRectangle.Height));*/
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

	for (int i = 0; i < placesToLookTo.size(); i++)
	{
		// look if Zero is here
		if(placesToLookTo[i]->getGlobalBounds().intersects(zeroRect))
		//if (placesToLookTo[i].Intersects(zeroCharacter.CollisionRectangle))
		{
			foundZero = true;
			break;
		}
	}
	if (foundZero)
	{
		// TODO
		// setMovementToTarget(zeroRect.left + zeroRect.width / 2, zeroRect.top + zeroRect.height / 2);
	}
	//else
	//{
	//	isMovementAnimationNeeded = false;
	//}
	MovingCharacter::update();
	updateDrawingObject();
}

Monster* Monster::createMonster(MONSTER_TYPES type, double x, double y, double w, double h)
{
	switch (type)
	{
	case MONSTER_TYPES::JELLY_MONSTER_TYPE:
		return new MonsterJelly(x, y, w, h);
		break;
	case MONSTER_TYPES::ONE_EYE_MONSTER_TYPE:
		return new MonsterOneEye(x, y, w, h);
		break;
	case MONSTER_TYPES::WALKING_SQUARE_MONSTER_TYPE:
		return new MonsterWalkingSquare(x, y, w, h);
		break;
	default:
		break;
	}
	return nullptr;
}
