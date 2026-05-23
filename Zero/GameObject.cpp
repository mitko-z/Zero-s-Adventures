#include "GameObject.h"

#include "StateMachine.h"
#include "SoundsPlayer.h"

GameObject::~GameObject() {}

std::ostringstream GameObject::getCurrentState()
{
	std::ostringstream oss;
	std::string commentBeginning = "Game object ";
	oss << addLineForOSS(std::to_string(getType()), true, commentBeginning + "type");
	oss << addLineForOSS(
		std::to_string(m_rect.x) + "," + 
		std::to_string(m_rect.y) + "," + 
		std::to_string(m_rect.w) + "," + 
		std::to_string(m_rect.h), 
		true, 
		commentBeginning + "rectangle x, y, width, height");
	oss << addLineForOSS(m_drawingObject.filePath ,true, commentBeginning + "path to texture file");
	oss << addLineForOSS(
		std::to_string(m_frames.framesAlongX) + "," + 
		std::to_string(m_frames.framesAlongY), 
		true, 
		commentBeginning + "animation frames");
	oss << addLineForOSS(std::to_string(static_cast<int>(m_isAnimating)), true, commentBeginning + "if is animated");
	oss << addLineForOSS(m_pathToMusicBackground, true, commentBeginning + "path to music background");
	oss << addLineForOSS(std::to_string(static_cast<int>(m_isActive)), true, commentBeginning + "if is active");
	return oss;
}

GameObject::GameObject(GameObject &other) : 
	m_rect(other.m_rect), 
	m_drawingObject(other.m_drawingObject),
	m_controllingKeys(other.m_controllingKeys),
	m_isAnimating(other.m_isAnimating)
{
	m_drawingObject.sprite.value().setTexture(other.m_drawingObject.sprite.value().getTexture());
}

void GameObject::initialize() {}

void GameObject::loadContent()
{
	extern std::shared_ptr <ResourcesManager> resMan;
	OBJ_TYPE gameObjType = getType();
	resMan->getAnimation(gameObjType, m_frames);
	m_drawingObject.filePath = resMan->getTexture(gameObjType).first;
	m_drawingObject.texture = resMan->getTexture(gameObjType).second;
	
	// Initialize the sprite with the texture
	m_drawingObject.sprite.emplace(m_drawingObject.texture);
	
	m_drawingObject.sprite.value().setPosition(sf::Vector2f(m_rect.x, m_rect.y));
	scaleSpriteTo(m_rect.w, m_rect.h, m_drawingObject.texture, m_drawingObject.sprite.value());
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
	window.draw(m_drawingObject.sprite.value());
}

void GameObject::updateDrawingObject()
{
	if (m_drawingObject.sprite.has_value())
	{
		m_drawingObject.sprite.value().setPosition(sf::Vector2f(m_rect.x, m_rect.y));
		updateAnimFrame();
	}
}

void GameObject::updateAnimFrame()
{
	if (!m_drawingObject.sprite.has_value())
		return;

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
	m_drawingObject.sprite.value().setTextureRect(sf::IntRect{
		sf::Vector2<int>(x, y),
		sf::Vector2<int>(width, height)
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

std::string GameObject::addLineForOSS(std::string data, bool addComment, std::string comment)
{
	std::string buffer = "";
	if (addComment)
		buffer = "; " + comment + "\n";
	buffer += data + "\n";
	return std::string(buffer);
}

