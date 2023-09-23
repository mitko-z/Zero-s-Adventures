#pragma once

#include <map>

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
		bool centerHorisontal = true);

	void loadContent() override;
	Definitions::ObjectType getType() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;

	bool pressed() { return m_isPressed; }
	void press();
	bool isActive() { return m_isActiveButton; }
	void activate() { m_isActiveButton = true; }
	void deactivate() { m_isActiveButton = false; }
protected:
	std::string m_textToDisplay;
	bool m_isPressed;
	sf::Text m_text;
private:
	bool m_isActiveButton;
	RUN_GAME_STATE m_runningGameState;
	sf::Font m_font;
	BUTTON_TYPE m_type;
	bool m_centerHorisontal;
	enum menubuttonSoundType
	{
		MENUBUTTON_SOUND_TYPE_PRESSED = 0,
	};
	const std::map<Definitions::ButtonType, std::string> m_buttonTypeToText
	{
		{BUTTON_TYPE::START_GAME_BUTTON, "START GAME"},
		{BUTTON_TYPE::EXIT_GAME_BUTTON,  "EXIT GAME"},
		{BUTTON_TYPE::SAVE_GAME_BUTTON,  "SAVE GAME"},
		{BUTTON_TYPE::SAVE_SLOT,         ""}, // @TODO to be defined in the Save Game?
		{BUTTON_TYPE::BACK_TO_MAIN,      "BACK TO MAIN MENU"},
	};

};