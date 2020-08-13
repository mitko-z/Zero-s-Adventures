#include "Menu.h"
#include "EventsHolder.h"

void Menu::initialize()
{
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::W, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Up, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::S, false));		// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Down, false));	// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Space, false));	// Choice
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Return, false));	// Choice
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Escape, false));	// Return to previous
	activeButtonIndex = 0;
	buttons[activeButtonIndex]->activate(); // make start button active
	setHighlighter(0);
}

Definitions::ObjectType Menu::getLoadResourcesCommand()
{
	return Definitions::ObjectType::MENU_TYPE;
}

void Menu::update()
{
	if (m_controllingKeys[sf::Keyboard::Up] || m_controllingKeys[sf::Keyboard::W])
	{
		activatePrevButton();
		// unregister the event
		m_controllingKeys[sf::Keyboard::Up] = false; 
		m_controllingKeys[sf::Keyboard::W] = false;
	}
	if (m_controllingKeys[sf::Keyboard::Down] || m_controllingKeys[sf::Keyboard::S])
	{
		activateNextButton();
		m_controllingKeys[sf::Keyboard::Down] = false;
		m_controllingKeys[sf::Keyboard::S] = false;
	}
	if (m_controllingKeys[sf::Keyboard::Space] || m_controllingKeys[sf::Keyboard::Return])
	{
		buttons[activeButtonIndex]->press();
		m_controllingKeys[sf::Keyboard::Space] = false;
		m_controllingKeys[sf::Keyboard::Return] = false;
	}

	for (auto button : buttons)
	{
		button->update();
	}

	highlighter.update();
}

void Menu::activatePrevButton()
{
	buttons[activeButtonIndex]->deactivate();
	activeButtonIndex--;
	if (activeButtonIndex < 0)
		activeButtonIndex = buttons.size() - 1;
	buttons[activeButtonIndex]->activate();

	setHighlighter(activeButtonIndex);
}

void Menu::activateNextButton()
{
	buttons[activeButtonIndex]->deactivate();
	activeButtonIndex++;
	if (activeButtonIndex >= buttons.size())
		activeButtonIndex = 0;
	buttons[activeButtonIndex]->activate();

	setHighlighter(activeButtonIndex);
}

void Menu::draw(sf::RenderWindow &window)
{
	GameObject::draw(window);

	for (auto button : buttons)
	{
		button->draw(window);
	}

	highlighter.draw(window);
}

void Menu::loadContent()
{
	GameObject::loadContent();
	for (auto button : buttons)
	{
		button->loadContent();
	}

	highlighter.loadContent();

}

void Menu::updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased)
{
	if (this->m_controllingKeys.size() > 0)
	{
		for (const auto& key : keysReleased)
		{
			if (this->m_controllingKeys.find(key.first) != this->m_controllingKeys.end())
			{
				this->m_controllingKeys[key.first] = key.second;
			}
		}
	}
}

void Menu::setAndInsertButtons(const std::vector<Definitions::ButtonType>& buttonTypes)
{
	if (buttonTypes.size() > 0)
	{
		const double scale = 0.7;
		const double w = m_rect.w * scale;
		const double x = (m_rect.w - w) / 2;
		const size_t buttonsNum = buttonTypes.size();
		const int allSpaces = 2 * buttonsNum + 1;
		const double h = m_rect.h / allSpaces;
		for (size_t i = 0; i < buttonsNum; ++i)
		{
			std::string text;
			switch (buttonTypes[i])
			{
				case Definitions::ButtonType::START_GAME_BUTTON:
					text = "START GAME";
				break;
				case Definitions::ButtonType::EXIT_GAME_BUTTON:
					text = "EXIT GAME";
				break;
				default:
				break;
			}
			double y = (2 * i + 1) * h;
			buttons.push_back(new MenuButton(x, y, w, h, false, buttonTypes[i], text));
		}
	}
}

void Menu::setHighlighter(size_t buttonIndex)
{
	if (buttons.size() > 0)
	{
		const double scale = 1.05;
		const Rectangle buttonRect = buttons[buttonIndex]->getRect();
		const double w = buttonRect.w * scale;
		const double h = buttonRect.h * scale;
		const double x = buttonRect.x - (w - buttonRect.w) / 2;
		const double y = buttonRect.y - (h - buttonRect.h) / 2;
		highlighter.setRect(Rectangle(x, y, w, h));
	}
}