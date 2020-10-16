#include "ZeroCharacter.h"
#include "Monster.h"

ZeroCharacter::ZeroCharacter(double x, double y, double w, double h, double speed, double health) :
	PlayingCharacter(x, y, w, h, false, speed, 0, health, 0), // 0 for damage & attacking speed - zero cannot make damage without a weapon
	m_takeWeapon(false),
	m_weapon(nullptr)
{
}

OBJ_TYPE ZeroCharacter::getLoadResourcesCommand()
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
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::W, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Up, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::S, false));		// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Down, false));	// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::A, false));		// move left
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Left, false));	// move left
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::D, false));		// move right
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Right, false));	// move right
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Space, false));	// fire
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::LControl, false));// fire
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::F, false));		// take weapon/item
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Return, false));	// take weapon/item
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::R, false));		// drop weapon
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::BackSpace, false));// drop weapon
#pragma endregion
}

// disabled - the base functionality works for now fine for this
//void ZeroCharacter::updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &keysPressed)
//{
//	
//}

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
	if (m_controllingKeys[sf::Keyboard::S] || m_controllingKeys[sf::Keyboard::Down])
	{
		setDirectionToMove(MovingDirection::DIRECTION_DOWN);
	}
	if (m_controllingKeys[sf::Keyboard::W] || m_controllingKeys[sf::Keyboard::Up])
	{
		setDirectionToMove(MovingDirection::DIRECTION_UP);
	}
	if (m_controllingKeys[sf::Keyboard::A] || m_controllingKeys[sf::Keyboard::Left])
	{
		setDirectionToMove(MovingDirection::DIRECTION_LEFT);
	}
	if (m_controllingKeys[sf::Keyboard::D] || m_controllingKeys[sf::Keyboard::Right])
	{
		setDirectionToMove(MovingDirection::DIRECTION_RIGHT);
	}
	if (m_controllingKeys[sf::Keyboard::F] || m_controllingKeys[sf::Keyboard::Return])
	{
		m_takeWeapon = true;
	}
	else
	{
		m_takeWeapon = false;
	}
	if (m_controllingKeys[sf::Keyboard::R] || m_controllingKeys[sf::Keyboard::BackSpace])
	{
		if (m_weapon)
		{
			m_weapon->setIsOwned(false);
			m_weapon->setRect(Rectangle(m_rect.x, m_rect.y, m_weapon->getRect().w, m_weapon->getRect().h));
			m_weapon = nullptr;
		}
	}

	PlayingCharacter::update();
}

void ZeroCharacter::processCollisions()
{
	for (auto* colidedObj : m_objsColideWith)
	{
		switch (colidedObj->getLoadResourcesCommand())
		{
		case OBJ_TYPE::MONSTER_TYPE:
			takeDamage(dynamic_cast<Monster*>(colidedObj)->getDamage());
			break;
		case OBJ_TYPE::WEAPON_TYPE:
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
