#include "LevelHeader.h"
#include "StateMachine.h"
#include "ResourcesManager.h"

void LevelHeader::loadContent()
{
		// timer
	setFontStyleToText(m_timerText);
	m_timerText.setPosition(m_rect.w / 2, m_rect.y);
		// Zero health info
	setFontStyleToText(m_zeroHealthText);
	m_zeroHealthText.setPosition(m_rect.x, m_rect.y);
		// Zero weapon info
	setFontStyleToText(m_zeroWeaponText);
	m_zeroWeaponText.setString("Weapon: ");
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
	window.draw(m_timerText);
	window.draw(m_zeroHealthText);
	drawZeroWeaponInfo(window);
}

void LevelHeader::setFontStyleToText(sf::Text& text)
{
	std::string loadPath;
	loadPath = "Data/Fonts/SMARC___.TTF";
	if (m_font.loadFromFile(loadPath) == 0)
	{
		std::string throwMessage = "Cannot load font " + loadPath;
		throw throwMessage;
	}
	text.setFont(m_font);
	sf::Color textColor(70, 255, 0);
	text.setFillColor(textColor);
	double fontSize = m_rect.h * 0.6;
	text.setCharacterSize(fontSize);
}

void LevelHeader::updateTimer()
{
	if (!m_timer.isStarted())
	{
		m_timer.start();
	}

	double currentTime = m_timer.elapsedSeconds();
	double elapsedTime = m_timerDuration - currentTime;
	double elapsedMinutes = static_cast<int>(elapsedTime / 60);
	double elapsedSeconds = (elapsedTime / 60 - elapsedMinutes) * 60;
	int elapsedMinutes_int = static_cast<int>(elapsedMinutes);
	int elapsedSeconds_int = static_cast<int>(elapsedSeconds);
	std::string elapsedMinutes_str = std::to_string(elapsedMinutes_int);
	std::string elapsedSeconds_str = std::to_string(elapsedSeconds_int);
	std::string leadingZero = elapsedSeconds_int < 10 ? "0" : "";
	m_timerText.setString(elapsedMinutes_str + ":" + leadingZero + elapsedSeconds_str);

	if (elapsedMinutes_int == 0 && elapsedSeconds_int < 15)
	{
		m_timerText.setFillColor(sf::Color(255, 70, 0));
	}
	if (elapsedMinutes_int == 0 && elapsedSeconds_int <= 10 && elapsedSeconds_int != m_prevElapsedSeconds)
	{
		playSound(OBJ_TYPE::LEVEL_HEADER_TYPE, 0);
	}
	m_prevElapsedSeconds = elapsedSeconds_int;

	if (static_cast<int>(elapsedTime) <= 0)
	{
		std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
		stateMachine->setEventByGameCommand(COMMAND::GAME_OVER_COMMAND);
	}
}

void LevelHeader::updateZeroHealth()
{
	int zeroHealth = m_zero->getCurrentHealth();
	m_zeroHealthText.setString("Health: " + std::to_string(zeroHealth));
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
		sf::Texture weaponTexture = resMan->getTexture(weaponType);
		m_zeroWeaponSprite = m_zero->getCurrentWeapon()->getWeaponSprite();
		scaleSpriteTo(
			resMan->getGameObjSize().x / 4,
			resMan->getGameObjSize().y / 4,
			*m_zeroWeaponSprite.getTexture(),
			m_zeroWeaponSprite);
		m_zeroWeaponSprite.setPosition(
			m_rect.w - (m_zeroWeaponSprite.getTexture()->getSize().x * m_zeroWeaponSprite.getScale().x), 
			m_rect.y);
			// set text
		m_zeroWeaponText.setString("Weapon: ");
		m_zeroWeaponText.setPosition(
			m_zeroWeaponSprite.getPosition().x - m_zeroWeaponText.getLocalBounds().width, 
			m_zeroWeaponSprite.getPosition().y);
	}
	else
	{
		m_zeroWeaponSprite = sf::Sprite();
		m_zeroWeaponText.setString("");
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
	window.draw(m_zeroWeaponSprite);
	window.draw(m_zeroWeaponText);
}
