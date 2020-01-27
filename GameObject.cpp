#include "GameObject.h"

GameObject::~GameObject() {}

GameObject::GameObject(GameObject &other) : 
	rect(other.rect), 
	drawingObject(other.drawingObject),
	controllingKeys(other.controllingKeys),
	isAnimating(other.isAnimating)
{
	drawingObject.sprite.setTexture(*other.drawingObject.sprite.getTexture());
}

void GameObject::initialize() {}

void GameObject::loadContent()
{
	extern ResourcesManager *resMan;
	LoadResourcesCommands loadCommand = getLoadResourcesCommand();
	resMan->getAnimation(loadCommand, frames);
	drawingObject.texture = resMan->getTexture(loadCommand);
	drawingObject.sprite.setPosition(rect.x, rect.y);
	drawingObject.sprite.setTexture(drawingObject.texture);
}

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

void GameObject::draw(sf::RenderWindow &window) 
{
	extern ResourcesManager *resMan;
	window.draw(drawingObject.sprite);
}

void GameObject::updateDrawingObject()
{
	drawingObject.sprite.setPosition(rect.x, rect.y);
	updateAnimFrame();
}

void GameObject::updateAnimFrame()
{
	if (!isAnimating)
	{
		animationFrame = 0;
	}
	else
	{
		if (++animationFrame > ((frames.framesAlongX * frames.framesAlongY) - 1))
		{
			animationFrame = 0;
		}
	}

	int width = drawingObject.texture.getSize().x / frames.framesAlongX;
	int height = drawingObject.texture.getSize().y / frames.framesAlongY;
	int x = (animationFrame % frames.framesAlongX) * width;
	int y = (animationFrame / frames.framesAlongX) * height;
	drawingObject.sprite.setTextureRect(sf::IntRect{
		x,
		y,
		width,
		height
	});
}

int GameObject::animationFrame = 0;