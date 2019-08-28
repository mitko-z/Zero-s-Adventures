#include "GameObject.h"

GameObject::~GameObject() {}

GameObject::GameObject(GameObject &other) : 
	rect(other.rect), 
	drawingObject(other.drawingObject),
	controllingKeys(other.controllingKeys)
{
	drawingObject.setTexture(other.drawingObject.getTexture());
}

void GameObject::initialize() {}

void GameObject::loadContent() {}

void GameObject::updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &controllingKeys)
{
	if (this->controllingKeys.size() > 0)
	{
		for (auto key : controllingKeys)
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