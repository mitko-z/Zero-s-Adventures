#include "GameObject.h"

#include "EventsHolder.h"

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
	Definitions::ObjectType loadCommand = getLoadResourcesCommand();
	resMan->getAnimation(loadCommand, frames);
	drawingObject.texture = resMan->getTexture(loadCommand);
	drawingObject.sprite.setPosition(rect.x, rect.y);
	drawingObject.sprite.setTexture(drawingObject.texture);
	scaleSpriteTo(rect.w, rect.h, drawingObject.texture, drawingObject.sprite);
}

Definitions::ObjectType GameObject::getLoadResourcesCommand()
{
	return Definitions::ObjectType::NO_OBJ_TYPE;
}

void GameObject::updateEvents()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	MAP_KEYS keysPressed = eventsHolder->getPressedKeys();
	MAP_KEYS keysReleased = eventsHolder->getReleasedKeys();
	updateKeys(keysPressed, keysReleased);
}

void GameObject::nullCollisions()
{
	hasCollisions = false;
	objsColideWith.clear();
}

void GameObject::setCollisionWith(GameObject& other)
{
	hasCollisions = true;
	objsColideWith.push_back(&other);
}

// by default detect on pressed key; might be overriden if needed
void GameObject::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (this->controllingKeys.size() > 0)
	{
		for (const auto& key : keysPressed)
		{
			if (this->controllingKeys.find(key.first) != this->controllingKeys.end())
			{
				this->controllingKeys[key.first] = true;
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

	int width = drawingObject.texture.getSize().x / frames.framesAlongY;
	int height = drawingObject.texture.getSize().y / frames.framesAlongX;
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
		static_cast<float>(w * frames.framesAlongX / texture.getSize().x),
		static_cast<float>(h * frames.framesAlongY / texture.getSize().y) };
	sprite.scale(factor);
}

