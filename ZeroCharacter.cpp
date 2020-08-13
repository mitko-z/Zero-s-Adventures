#include "ZeroCharacter.h"

ZeroCharacter::ZeroCharacter(double x, double y, double w, double h) : 
	MovingCharacter(x, y, w, h, IS_ANIMATING, ZERO_SPEED, 0) // 0 for damage - zero cannot make damage without a weapon
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
	if (this->m_controllingKeys[sf::Keyboard::S] || this->m_controllingKeys[sf::Keyboard::Down])
	{
		setDirectionToMove(MovingDirection::DIRECTION_DOWN);
	}
	if (this->m_controllingKeys[sf::Keyboard::W] || this->m_controllingKeys[sf::Keyboard::Up])
	{
		setDirectionToMove(MovingDirection::DIRECTION_UP);
	}
	if (this->m_controllingKeys[sf::Keyboard::A] || this->m_controllingKeys[sf::Keyboard::Left])
	{
		setDirectionToMove(MovingDirection::DIRECTION_LEFT);
	}
	if (this->m_controllingKeys[sf::Keyboard::D] || this->m_controllingKeys[sf::Keyboard::Right])
	{
		setDirectionToMove(MovingDirection::DIRECTION_RIGHT);
	}

	MovingCharacter::update();
	updateDrawingObject();
}

void ZeroCharacter::draw(sf::RenderWindow &window)
{
	GameObject::draw(window);
}