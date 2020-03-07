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
	Definitions::LoadResourcesCommands loadCommand = getLoadResourcesCommand();
	resMan->getAnimation(loadCommand, frames);
	drawingObject.texture = resMan->getTexture(loadCommand);
	drawingObject.sprite.setPosition(rect.x, rect.y);
	drawingObject.sprite.setTexture(drawingObject.texture);
}

Definitions::LoadResourcesCommands GameObject::getLoadResourcesCommand()
{
	return Definitions::LoadResourcesCommands::NONE;
}

void GameObject::updateEvents(
	const UMAP<sf::Keyboard::Key, bool>& keysPressed, 
	const UMAP<sf::Keyboard::Key, bool>& keysReleased)
{
	updateKeys(keysPressed); // by default detect on pressed key, might be overriden if needed
}

void GameObject::updateKeys(const UMAP<sf::Keyboard::Key, bool>& keys)
{
	if (this->controllingKeys.size() > 0)
	{
		for (auto key : keys)
		{
			if (this->controllingKeys.find(key.first) != this->controllingKeys.end())
			{
				this->controllingKeys[key.first] = key.second;
			}
		}
	}
}

void GameObject::update() 
{
	updateDrawingObject();
}

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

void GameObject::scaleSpriteTo(double w, double h, const sf::Texture& texture, sf::Sprite& sprite)
{
	sf::Vector2f factor{
		static_cast<float>(w / texture.getSize().x),
		static_cast<float>(h / texture.getSize().y) };
	sprite.scale(factor);
}

int GameObject::animationFrame = 0;