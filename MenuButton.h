#pragma once

#include "GameObject.h"
#include "Definitions.h"

class MenuButton : public GameObject
{
public:
	MenuButton() = default;
	MenuButton(
		double x,
		double y,
		double w,
		double h,
		bool isAnimating,
		Definitions::ButtonType type,
		const std::string& text);

	void loadContent() override;
	Definitions::ObjectType getType() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;

	bool pressed() { return m_isPressed; }
	void press();
	bool isActive() { return m_isActiveButton; }
	void activate() { m_isActiveButton = true; }
	void deactivate() { m_isActiveButton = false; }
private:
	bool m_isPressed;
	bool m_isActiveButton;
	RUN_GAME_STATE m_runningGameState;
	std::string m_textToDisplay;
	sf::Font m_font;
	sf::Text m_text;
	BUTTON_TYPE m_type;
	enum menubuttonSoundType
	{
		MENUBUTTON_SOUND_TYPE_PRESSED = 0,
	};
};