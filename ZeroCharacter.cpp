#include "ZeroCharacter.h"

ZeroCharacter::ZeroCharacter(double x, double y, double w, double h, bool isAnimating) : 
	GameObject(x, y, w, h, isAnimating)
{

}

Definitions::LoadResourcesCommand ZeroCharacter::getLoadResourcesCommand()
{
	return Definitions::LoadResourcesCommand::ZERO_RES;
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
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::W, false));		// move up
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::Up, false));		// move up
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::S, false));		// move down
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::Down, false));	// move down
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::A, false));		// move left
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::Left, false));	// move left
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::D, false));		// move right
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::Right, false));	// move right
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::Space, false));	// fire
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::LControl, false));// fire
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::F, false));		// take weapon/item
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::Return, false));	// take weapon/item
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::R, false));		// drop weapon
	this->controllingKeys.insert(std::make_pair(sf::Keyboard::BackSpace, false));// drop weapon
#pragma endregion

#pragma region drawing
	// TODO: these to be set precise later
	this->rect.x = 10;
	this->rect.y = 10;
	this->rect.h = 10;
	this->rect.w = 10;
	this->speed = 10;
#pragma endregion

}

// disabled - the base functionality works for now fine for this
//void ZeroCharacter::updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &keysPressed)
//{
//	
//}

void ZeroCharacter::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (controllingKeys.size() > 0)
	{
		for (const auto& key : keysPressed)
		{
			if (controllingKeys.find(key.first) != this->controllingKeys.end())
			{
				controllingKeys[key.first] = true;
			}
		}
	}

	if (this->controllingKeys.size() > 0)
	{
		for (const auto& key : keysReleased)
		{
			if (this->controllingKeys.find(key.first) != this->controllingKeys.end())
			{
				this->controllingKeys[key.first] = false;
			}
		}
	}
}

void ZeroCharacter::update()
{
	bool shouldAnimate = false;
	if (this->controllingKeys[sf::Keyboard::S] || this->controllingKeys[sf::Keyboard::Down])
	{
		this->rect.y += this->speed;
		shouldAnimate = true;
	}
	if (this->controllingKeys[sf::Keyboard::W] || this->controllingKeys[sf::Keyboard::Up])
	{
		this->rect.y -= this->speed;
		shouldAnimate = true;
	}
	if (this->controllingKeys[sf::Keyboard::A] || this->controllingKeys[sf::Keyboard::Left])
	{
		this->rect.x -= this->speed;
		shouldAnimate = true;
	}
	if (this->controllingKeys[sf::Keyboard::D] || this->controllingKeys[sf::Keyboard::Right])
	{
		this->rect.x += this->speed;
		shouldAnimate = true;
	}
	if (shouldAnimate)	isAnimating = true;
	else				isAnimating = false;

	updateDrawingObject();
}

void ZeroCharacter::draw(sf::RenderWindow &window)
{
	GameObject::draw(window);
}