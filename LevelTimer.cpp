#include "LevelTimer.h"
#include "StateMachine.h"

void LevelTimer::loadContent()
{
	std::string loadPath;
	// fonts
	loadPath = "Data/Fonts/SMARC___.TTF";
	if (m_font.loadFromFile(loadPath) == 0)
	{
		std::string throwMessage = "Cannot load font " + loadPath;
		throw throwMessage;
	}

	// texts
	m_text.setFont(m_font);
	m_text.setFillColor(sf::Color(70, 255, 0));
	m_text.setString(m_textToDisplay);
	double fontSize = m_rect.h * 0.6;
	m_text.setCharacterSize(fontSize);
	m_text.setPosition(m_rect.x, m_rect.y);
}

void LevelTimer::update()
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
	m_textToDisplay = elapsedMinutes_str + ":" + leadingZero + elapsedSeconds_str;
	m_text.setString(m_textToDisplay);
	
	if (elapsedMinutes_int == 0 && elapsedSeconds_int < 15)
	{
		m_text.setFillColor(sf::Color(255, 70, 0));
	}
	if (elapsedMinutes_int == 0 && elapsedSeconds_int <= 10 && elapsedSeconds_int != m_prevElapsedSeconds)
	{
		playSound(OBJ_TYPE::LEVEL_TIMER_TYPE, 0);
	}
	m_prevElapsedSeconds = elapsedSeconds_int;
	
	if (static_cast<int>(elapsedTime) <= 0)
	{
		std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
		stateMachine->setEventByGameCommand(COMMAND::GAME_OVER_COMMAND);
	}
}


void LevelTimer::draw(sf::RenderWindow &window)
{
	window.draw(m_text);
}
