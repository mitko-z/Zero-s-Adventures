#pragma once

#include "GameObject.h"
#include "Timer.h"
#include "ZeroCharacter.h"
#include <memory>

class LevelHeader : public GameObject
{
public:
	LevelHeader(double x, double y, double w, double h, double levelTimeDuration) :
		GameObject(x, y, w, h, false),
		m_timerDuration(levelTimeDuration),
		m_prevElapsedSeconds(0),
		m_zeroWeaponSprite(nullptr)
	{
		// Load font from file - required for SFML 3.x Text initialization
		std::string loadPath = "Data/Fonts/SMARC___.TTF";
		if (!m_font.openFromFile(loadPath)) {
			throw std::string("Cannot load font " + loadPath);
		}
		// Initialize Text objects with the loaded font using smart pointers
		m_timerText = std::make_unique<sf::Text>(m_font);
		m_zeroHealthText = std::make_unique<sf::Text>(m_font);
		m_zeroWeaponText = std::make_unique<sf::Text>(m_font);
	}

	OBJ_TYPE getType() override { return OBJ_TYPE::LEVEL_HEADER_TYPE; }

	void loadContent() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;
	std::ostringstream getCurrentState() override;

private:
	void setFontStyleToText(sf::Text& text);
	void setTextPosition(sf::Text& text, double x, double y);
	void setSpritePosition(sf::Sprite& sprite, double x, double y);
	void updateTimer();
	void updateZeroHealth();
	void updateZeroWeapon();
	void setZeroGameObject();
	void drawZeroWeaponInfo(sf::RenderWindow& window);

	sf::Font m_font;
	std::unique_ptr<sf::Text> m_timerText;
	Timer m_timer;
	double m_timerDuration;
	double m_elapsedTime;
	int m_prevElapsedSeconds;
	ZeroCharacter* m_zero;
	std::unique_ptr<sf::Text> m_zeroHealthText;
	std::unique_ptr<sf::Text> m_zeroWeaponText;
	std::unique_ptr<sf::Sprite> m_zeroWeaponSprite;
};