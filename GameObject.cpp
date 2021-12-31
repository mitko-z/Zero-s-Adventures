#include "GameObject.h"

#include "StateMachine.h"
#include "SoundsPlayer.h"

GameObject::~GameObject() {}

GameObject::GameObject(GameObject &other) : 
	m_rect(other.m_rect), 
	m_drawingObject(other.m_drawingObject),
	m_controllingKeys(other.m_controllingKeys),
	m_isAnimating(other.m_isAnimating)
{
	m_drawingObject.sprite.setTexture(*other.m_drawingObject.sprite.getTexture());
}

void GameObject::initialize() {}

void GameObject::loadContent()
{
	extern std::shared_ptr <ResourcesManager> resMan;
	OBJ_TYPE gameObjType = getType();
	resMan->getAnimation(gameObjType, m_frames);
	m_drawingObject.texture = resMan->getTexture(gameObjType);
	m_drawingObject.sprite.setPosition(m_rect.x, m_rect.y);
	m_drawingObject.sprite.setTexture(m_drawingObject.texture);
	scaleSpriteTo(m_rect.w, m_rect.h, m_drawingObject.texture, m_drawingObject.sprite);
}

OBJ_TYPE GameObject::getType()
{
	return OBJ_TYPE::NO_OBJ_TYPE;
}

void GameObject::updateEvents()
{
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	MAP_KEYS keysPressed = stateMachine->getPressedKeys();
	MAP_KEYS keysReleased = stateMachine->getReleasedKeys();
	updateKeys(keysPressed, keysReleased);
}

void GameObject::nullCollisions()
{
	m_hasCollisions = false;
	m_objsColideWith.clear();
}

void GameObject::setCollisionWith(GameObject& other)
{
	m_hasCollisions = true;
	m_objsColideWith.push_back(&other);
}

// by default detect on pressed key; might be overriden if needed
void GameObject::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (this->m_controllingKeys.size() > 0)
	{
		for (const auto& key : keysPressed)
		{
			if (this->m_controllingKeys.find(key.first) != this->m_controllingKeys.end())
			{
				this->m_controllingKeys[key.first] = true;
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
	extern std::shared_ptr <ResourcesManager> resMan;
	window.draw(m_drawingObject.sprite);
}

void GameObject::updateDrawingObject()
{
	m_drawingObject.sprite.setPosition(m_rect.x, m_rect.y);
	updateAnimFrame();
}

void GameObject::updateAnimFrame()
{
	if (!m_isAnimating)
	{
		m_animationFrame = 0;
	}
	else
	{
		if (++m_animationFrame > ((m_frames.framesAlongX * m_frames.framesAlongY) - 1))
		{
			m_animationFrame = 0;
		}
	}

	int width = m_drawingObject.texture.getSize().x / m_frames.framesAlongY;
	int height = m_drawingObject.texture.getSize().y / m_frames.framesAlongX;
	int x = (m_animationFrame % m_frames.framesAlongY) * width;
	int y = (m_animationFrame / m_frames.framesAlongY) * height;
	m_drawingObject.sprite.setTextureRect(sf::IntRect{
		x,
		y,
		width,
		height
	});
}

void GameObject::scaleSpriteTo(double w, double h, const sf::Texture& texture, sf::Sprite& sprite)
{
	float factor1 = w * m_frames.framesAlongY / texture.getSize().x;
	float factor2 = h * m_frames.framesAlongX / texture.getSize().y;
	sf::Vector2f factor{ factor1,factor2 };
	sprite.setScale(factor);
	//sprite.scale(factor);
}

void GameObject::playSound(OBJ_TYPE gameObjType, int rangeType)
{
	extern std::shared_ptr <ResourcesManager> resMan;
	bool isErr;
	SoundBuffersHolder *soundBuffersHolder = &resMan->getSoundBuffers(gameObjType, isErr);
	if (isErr)
	{
		std::string throwMessage = "Can't play sound for type " + gameObjType;
		throw throwMessage;
	}

	srand(time(NULL));
	int index = rand() % (soundBuffersHolder->ranges[rangeType]);
	SoundsPlayer::getInstance()->play(soundBuffersHolder->soundBuffers[index]);
}

