#include "ZeroCharacter.h"
#include "Monster.h"
#include "StateMachine.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

ZeroCharacter::ZeroCharacter(double x, double y, double w, double h, double speed, double health, double attackingSpeed, double firingAccuracy, bool isFlipped) :
	PlayingCharacter(x, y, w, h, false, speed, 0, health, attackingSpeed, isFlipped), // 0 for damage & attacking speed - zero cannot make damage without a weapon
	m_takeWeapon(false),
	m_weapon(nullptr),
	m_lastDirection(MovingDirection::DIRECTION_NONE),
	m_firingAccuracy(firingAccuracy)
{
}

OBJ_TYPE ZeroCharacter::getType()
{
	return OBJ_TYPE::ZERO_TYPE;
}

void ZeroCharacter::initialize()
{
#pragma region controlling_keys
	// define keys which control Zero's movement.
	// they must be defined in pairs in strict order; one pair controls one action:
	// 0 and 1 - move up
	// 2, 3 - move down
	// 4, 5 - left
	// 6, 7 - right
	// 8, 9 - fire
	// 10, 11 - take item from the ground (which is not owned by anyone else)
	// 12, 13 - leaves the item
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::W, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Up, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::S, false));		// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Down, false));	// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::A, false));		// move left
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Left, false));	// move left
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::D, false));		// move right
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Right, false));	// move right
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Space, false));	// fire
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::LControl, false));// fire
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::F, false));		// take weapon/item
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Enter, false));	// take weapon/item
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::R, false));		// drop weapon
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Key::Backspace, false));// drop weapon
#pragma endregion
}

// disabled - the base functionality works for now fine for this
//void ZeroCharacter::updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &keysPressed)
//{
//	
//}

void ZeroCharacter::setIsActive()
{
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	int health = static_cast<int>(getCurrentHealth());
	if (health <= 0)
	{
		stateMachine->setEventByGameCommand(COMMAND::GAME_OVER_COMMAND);
	}
}

std::ostringstream ZeroCharacter::getCurrentState()
{
	std::ostringstream oss = PlayingCharacter::getCurrentState();
	std::string commentBeginning = "Zero character ";
	oss << addLineForOSS(std::to_string(m_firingAccuracy), true, commentBeginning + "firing accuracy");
	return oss;
}

void ZeroCharacter::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (m_controllingKeys.size() > 0)
	{
		for (const auto& key : keysPressed)
		{
			if (m_controllingKeys.find(key.first) != this->m_controllingKeys.end())
			{
				m_controllingKeys[key.first] = true;
			}
		}
	}

	if (this->m_controllingKeys.size() > 0)
	{
		for (const auto& key : keysReleased)
		{
			if (this->m_controllingKeys.find(key.first) != this->m_controllingKeys.end())
			{
				this->m_controllingKeys[key.first] = false;
			}
		}
	}
}

void ZeroCharacter::update()
{
	if (m_controllingKeys[sf::Keyboard::Key::S] || m_controllingKeys[sf::Keyboard::Key::Down])
	{
		setDirectionToMove(MovingDirection::DIRECTION_DOWN);
	}
	if (m_controllingKeys[sf::Keyboard::Key::W] || m_controllingKeys[sf::Keyboard::Key::Up])
	{
		setDirectionToMove(MovingDirection::DIRECTION_UP);
	}
	if (m_controllingKeys[sf::Keyboard::Key::A] || m_controllingKeys[sf::Keyboard::Key::Left])
	{
		setDirectionToMove(MovingDirection::DIRECTION_LEFT);
	}
	if (m_controllingKeys[sf::Keyboard::Key::D] || m_controllingKeys[sf::Keyboard::Key::Right])
	{
		setDirectionToMove(MovingDirection::DIRECTION_RIGHT);
	}
	if (m_controllingKeys[sf::Keyboard::Key::F] || m_controllingKeys[sf::Keyboard::Key::Enter])
	{
		m_takeWeapon = true;
	}
	else
	{
		m_takeWeapon = false;
	}
	if (m_controllingKeys[sf::Keyboard::Key::R] || m_controllingKeys[sf::Keyboard::Key::Backspace])
	{
		if (m_weapon)
		{
			m_weapon->setIsOwned(false);
			m_weapon->setRect(Rectangle(m_rect.x, m_rect.y, m_weapon->getRect().w, m_weapon->getRect().h));
			m_weapon = nullptr;
		}
	}
	if (m_controllingKeys[sf::Keyboard::Key::LControl] || m_controllingKeys[sf::Keyboard::Key::Space])
	{
		if (m_weapon)
		{
			attack();
			m_isAnimating = true;
		}
	}
	else
	{
		stopAttack();
		m_isAnimating = false;
	}

	if (m_weapon)
	{
		m_weapon->setRect(this->m_rect);
	}

	if (m_directionToMove != MovingDirection::DIRECTION_NONE)
	{
		m_lastDirection = m_directionToMove;
	}

	PlayingCharacter::update();
}

void ZeroCharacter::processCollisions()
{
	for (auto* colidedObj : m_objsColideWith)
	{
		switch (colidedObj->getType())
		{
		case OBJ_TYPE::MONSTER_TYPE:
			takeDamage(dynamic_cast<Monster*>(colidedObj)->getDamage());
			break;
		case OBJ_TYPE::BOW_WEAPON_TYPE:
		case OBJ_TYPE::FIREBALL_WEAPON_TYPE:
		case OBJ_TYPE::STAR_WEAPON_TYPE:
			if (m_takeWeapon)
			{
				if (!m_weapon)
				{
					m_weapon = dynamic_cast<Weapon*>(colidedObj);
					m_weapon->setIsOwned(true);
				}
			}
		default:
			break;
		}
	}
	PlayingCharacter::processCollisions();
}

void ZeroCharacter::attack()
{
	if (PlayingCharacter::startAttack() || canMakeNextAttack())
	{
		if (m_weapon)
		{
			double firingAngle;
			calculateFiringAngle(firingAngle);
			m_weapon->fire(firingAngle);
		}
	}
}

void ZeroCharacter::calculateFiringAngle(double& angle)
{
	if (m_lastDirection == MovingDirection::DIRECTION_NONE)
		angle = MovingDirection::DIRECTION_RIGHT;
	else
		angle = m_lastDirection;
	// adding randomness
	srand(time(NULL));
	int errorRange = 100 - m_firingAccuracy;
	int randomness = rand() % (errorRange) + 1;
	int inaccuracy = randomness - errorRange / 2;

	angle += inaccuracy;
}