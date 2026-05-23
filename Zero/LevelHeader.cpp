#include "LevelHeader.h"
#include "StateMachine.h"
#include "ResourcesManager.h"

void LevelHeader::loadContent()
{
		// timer
	setFontStyleToText(*m_timerText);
	setTextPosition(*m_timerText, m_rect.w / 2, m_rect.y);
		// Zero health info
	setFontStyleToText(*m_zeroHealthText);
	setTextPosition(*m_zeroHealthText, m_rect.x, m_rect.y);
		// Zero weapon info
	setFontStyleToText(*m_zeroWeaponText);
	m_zeroWeaponText->setString("Weapon: ");
}

void LevelHeader::update()
{
	updateTimer();
	setZeroGameObject();
	updateZeroHealth();
	updateZeroWeapon();
}


void LevelHeader::draw(sf::RenderWindow &window)
{
	window.draw(*m_timerText);
	window.draw(*m_zeroHealthText);
	drawZeroWeaponInfo(window);
}

std::ostringstream LevelHeader::getCurrentState()
{
	std::ostringstream oss = GameObject::getCurrentState();
	oss << addLineForOSS(std::to_string(m_elapsedTime), true, "Level Header elapsed time");

	return oss;
}

void LevelHeader::setFontStyleToText(sf::Text& text)
{
	text.setFont(m_font);
	sf::Color textColor(70, 255, 0);
	text.setFillColor(textColor);
	double fontSize = m_rect.h * 0.6;
	text.setCharacterSize(static_cast<unsigned int>(fontSize));
}

void LevelHeader::setTextPosition(sf::Text& text, double x, double y)
{
	text.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
}

void LevelHeader::setSpritePosition(sf::Sprite& sprite, double x, double y)
{
	sprite.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
}

void LevelHeader::updateTimer()
{
	if (!m_timer.isStarted())
	{
		m_timer.start();
	}

	double currentTime = m_timer.elapsedSeconds();
	m_elapsedTime = m_timerDuration - currentTime;
	double elapsedMinutes = static_cast<int>(m_elapsedTime / 60);
	double elapsedSeconds = (m_elapsedTime / 60 - elapsedMinutes) * 60;
	int elapsedMinutes_int = static_cast<int>(elapsedMinutes);
	int elapsedSeconds_int = static_cast<int>(elapsedSeconds);
	std::string elapsedMinutes_str = std::to_string(elapsedMinutes_int);
	std::string elapsedSeconds_str = std::to_string(elapsedSeconds_int);
	std::string leadingZero = elapsedSeconds_int < 10 ? "0" : "";
	m_timerText->setString(elapsedMinutes_str + ":" + leadingZero + elapsedSeconds_str);

	if (elapsedMinutes_int == 0 && elapsedSeconds_int < 15)
	{
		m_timerText->setFillColor(sf::Color(255, 70, 0));
	}
	if (elapsedMinutes_int == 0 && elapsedSeconds_int <= 10 && elapsedSeconds_int != m_prevElapsedSeconds)
	{
		playSound(OBJ_TYPE::LEVEL_HEADER_TYPE, 0);
	}
	m_prevElapsedSeconds = elapsedSeconds_int;

	if (static_cast<int>(m_elapsedTime) <= 0)
	{
		std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
		stateMachine->setEventByGameCommand(COMMAND::GAME_OVER_COMMAND);
	}
}

void LevelHeader::updateZeroHealth()
{
	int zeroHealth = m_zero->getCurrentHealth();
	m_zeroHealthText->setString("Health: " + std::to_string(zeroHealth));
}

void LevelHeader::updateZeroWeapon()
{
	Weapon* zeroWeapon = m_zero->getCurrentWeapon();
	if (zeroWeapon)
	{
			// set image
		extern std::shared_ptr <ResourcesManager> resMan;
		OBJ_TYPE weaponType = zeroWeapon->getType();
		Animation frames;
		resMan->getAnimation(weaponType, frames);
		sf::Texture weaponTexture = resMan->getTexture(weaponType).second;
		
		// Create new sprite if needed or update existing one
		const sf::Sprite* weaponSpritePtr = m_zero->getCurrentWeapon()->getWeaponSprite();
		
		if (weaponSpritePtr)
		{
			sf::Sprite weaponSprite(*weaponSpritePtr);
			
			scaleSpriteTo(
				resMan->getGameObjSize().x / 4,
				resMan->getGameObjSize().y / 4,
				weaponSprite.getTexture(),
				weaponSprite);
			setSpritePosition(
				weaponSprite, 
				m_rect.w - (weaponSprite.getTexture().getSize().x * weaponSprite.getScale().x), 
				m_rect.y);
			
			// Update or create the member sprite using smart pointer
			m_zeroWeaponSprite = std::make_unique<sf::Sprite>(weaponSprite);
		}
		
			// set text
		m_zeroWeaponText->setString("Weapon: ");
		setTextPosition(
			*m_zeroWeaponText,
			m_zeroWeaponSprite->getPosition().x - m_zeroWeaponText->getLocalBounds().size.x, 
			m_zeroWeaponSprite->getPosition().y);
	}
	else
	{
		// No weapon - clean up sprite (smart pointer handles cleanup automatically)
		m_zeroWeaponSprite = nullptr;
		m_zeroWeaponText->setString("");
	}
}

void LevelHeader::setZeroGameObject()
{
	extern std::shared_ptr<ResourcesManager> resMan;
	auto gameObjs = resMan->getGameObjects();
	for (auto& gameObj : gameObjs)
	{
		if (gameObj->getType() == OBJ_TYPE::ZERO_TYPE)
		{
			m_zero = dynamic_cast<ZeroCharacter*>(gameObj);
			break;
		}
	}
}

void LevelHeader::drawZeroWeaponInfo(sf::RenderWindow & window)
{
	extern std::shared_ptr<ResourcesManager> resMan;
	if (m_zeroWeaponSprite != nullptr)
	{
		window.draw(*m_zeroWeaponSprite);
	}
	window.draw(*m_zeroWeaponText);
}
