#include "GameObject.h"

GameObject::~GameObject() {}

GameObject::GameObject(GameObject &other) : 
	rect(other.rect), 
	drawingObject(other.drawingObject),
	controllingKeys(other.controllingKeys)
{
	drawingObject.rectangleShape.setTexture(other.drawingObject.rectangleShape.getTexture());
}

void GameObject::initialize() {}

void GameObject::loadContent() {}

LoadResourcesCommands GameObject::getLoadResourcesCommand()
{
	return LoadResourcesCommands::NONE;
}

void GameObject::updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &keysPressed)
{
	if (this->controllingKeys.size() > 0)
	{
		for (auto key : keysPressed)
		{
			if (this->controllingKeys.find(key.first) != this->controllingKeys.end())
			{
				this->controllingKeys[key.first] = key.second;
			}
		}
	}
}

void GameObject::update() {}

void GameObject::draw(sf::RenderWindow &window) {}

void GameObject::updateDrawingObject()
{
	this->drawingObject.rectangleShape.setPosition(sf::Vector2f(rect.x, rect.y));
}