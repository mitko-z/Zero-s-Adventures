#include "ZeroCharacter.h"

ZeroCharacter::ZeroCharacter(int x, int y, int w, int h) : GameObject(x, y, w, h)
{

}

void ZeroCharacter::loadContent()
{
	std::string loadPath = "Data/Images/ZeroSingleImage.png";
	if (!texture.loadFromFile(loadPath))
	{
		std::string throwMessage = "Cannot load image " + loadPath;
		throw throwMessage;
	}

	drawingObject.setPosition(this->rect.x, this->rect.y);
	drawingObject.setSize(sf::Vector2f(texture.getSize()));
	drawingObject.setTexture(&texture);
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
#pragma endregion

}
void ZeroCharacter::updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &controllingKeys)
{
	// TODO
}

void ZeroCharacter::update()
{
	// TODO
}

void ZeroCharacter::draw(sf::RenderWindow &window)
{
	window.draw(drawingObject);
}