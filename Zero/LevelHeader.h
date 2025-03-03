#pragma once

#include "GameObject.h"
#include "Timer.h"
#include "ZeroCharacter.h"

class LevelHeader : public GameObject
{
public:
	LevelHeader(double x, double y, double w, double h, double levelTimeDuration) :
		GameObject(x, y, w, h, false),
		m_timerDuration(levelTimeDuration),
		m_prevElapsedSeconds(0)
	{}

	OBJ_TYPE getType() override { return OBJ_TYPE::LEVEL_HEADER_TYPE; }

	void loadContent() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;
	std::ostringstream getCurrentState() override;

private:
	void setFontStyleToText(sf::Text& text);
	void updateTimer();
	void updateZeroHealth();
	void updateZeroWeapon();
	void setZeroGameObject();
	void drawZeroWeaponInfo(sf::RenderWindow& window);

	sf::Font m_font;
	sf::Text m_timerText;
	Timer m_timer;
	double m_timerDuration;
	double m_elapsedTime;
	int m_prevElapsedSeconds;
	ZeroCharacter* m_zero;
	sf::Text m_zeroHealthText;
	sf::Text m_zeroWeaponText;
	sf::Sprite m_zeroWeaponSprite;
};