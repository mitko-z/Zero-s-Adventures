#include "MenuButton.h"

#include "EventsHolder.h"

MenuButton::MenuButton(	double x, 
						double y, 
						double w, 
						double h, 
						bool isAnimating, 
						Definitions::ButtonType type,
						const std::string& text) :
	GameObject(x, y, w, h, isAnimating), 
	m_isPressed(false), 
	m_type(type),
	m_textToDisplay(text),
	m_isActiveButton(false),
	m_runningGameState(RUN_GAME_STATE::PLAYING_STATE)
{
}

void MenuButton::loadContent()
{

	GameObject::loadContent();

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
	// center m_text in the middle along x...
	double posX = m_rect.x + (m_rect.w / 2) - (m_textToDisplay.size() / 2) * m_text.getCharacterSize() * 0.5;
	// ... and along y
	double posY = m_rect.y + (m_rect.h / 2) - m_text.getCharacterSize() * 0.7;
	m_text.setPosition(posX, posY);
}

Definitions::ObjectType MenuButton::getType()
{
	return Definitions::ObjectType::MENU_BUTTON_TYPE;
}

void MenuButton::update()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	if (m_isPressed)
	{
		eventHolder->setEventByButton(m_type);
		m_isPressed = false;
	}
}


void MenuButton::draw(sf::RenderWindow &window)
{
	GameObject::draw(window);
	window.draw(m_text);
}

void MenuButton::press()
{
	m_isPressed = true;

	playSound(OBJ_TYPE::MENU_BUTTON_TYPE, menubuttonSoundType::MENUBUTTON_SOUND_TYPE_PRESSED);
}