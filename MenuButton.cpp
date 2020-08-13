#include "MenuButton.h"

#include "EventsHolder.h"

MenuButton::MenuButton(	double x, 
						double y, 
						double w, 
						double h, 
						bool isAnimating, 
						Definitions::ButtonType type,
						std::string text) :
	GameObject(x, y, w, h, isAnimating), 
	isPressed_(false), 
	type(type),
	textToDisplay(text)
{
}

void MenuButton::loadContent()
{

	GameObject::loadContent();

	std::string loadPath;
	// fonts
	loadPath = "Data/Fonts/SMARC___.TTF";
	if (font.loadFromFile(loadPath) == 0)
	{
		std::string throwMessage = "Cannot load font " + loadPath;
		throw throwMessage;
	}

	// texts
	text.setFont(font);
	text.setFillColor(sf::Color(70, 255, 0));
	text.setString(textToDisplay);
	double fontSize = m_rect.h * 0.6;
	text.setCharacterSize(fontSize);
	// center text in the middle along x...
	double posX = m_rect.x + (m_rect.w / 2) - (textToDisplay.size() / 2) * text.getCharacterSize() * 0.5;
	// ... and along y
	double posY = m_rect.y + (m_rect.h / 2) - text.getCharacterSize() * 0.7;
	text.setPosition(posX, posY);
}

Definitions::ObjectType MenuButton::getLoadResourcesCommand()
{
	return Definitions::ObjectType::MENU_BUTTON_TYPE;
}

void MenuButton::update()
{
	std::shared_ptr<EventsHolder> eventHolder = EventsHolder::getInstnce();
	if (isPressed_)
	{
		eventHolder->setEventByButton(type);
		isPressed_ = false;
	}
}


void MenuButton::draw(sf::RenderWindow &window)
{
	GameObject::draw(window);
	window.draw(text);
}

void MenuButton::press()
{
	isPressed_ = true;
}