#pragma once

#include "GameObject.h"
#include "Timer.h"

class LevelTimer : public GameObject
{
public:
	LevelTimer(double x, double y, double w, double h, double levelDuration) :
		GameObject(x, y, w, h, false),
		m_timerDuration(levelDuration),
		m_prevElapsedSeconds(0)
	{}

	OBJ_TYPE getType() override { return OBJ_TYPE::LEVEL_TIMER_TYPE; }

	void loadContent() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;

private:
	sf::Font m_font;
	sf::Text m_text;
	std::string m_textToDisplay;
	Timer m_timer;
	double m_timerDuration;
	int m_prevElapsedSeconds;
};