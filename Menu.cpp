#pragma once

#include "Menu.h"
#include "BackgroundAudioPlayer.h"
#include "StateMachine.h"
#include "MenuButton.h"
#include "SaveSlotButton.h"

void Menu::initialize()
{
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::W, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Up, false));		// move up
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::S, false));		// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Down, false));	// move down
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Space, false));	// Choice
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Return, false));	// Choice
	this->m_controllingKeys.insert(std::make_pair(sf::Keyboard::Escape, false));	// Return to previous
	m_activeButtonIndex = 0;
	if (m_buttons.size() > 0)
	{
		m_buttons[m_activeButtonIndex]->activate(); // make start button active
		setHighlighter(0);
	}


}

Definitions::ObjectType Menu::getType()
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
		m_buttons[m_activeButtonIndex]->press();
		m_controllingKeys[sf::Keyboard::Space] = false;
		m_controllingKeys[sf::Keyboard::Return] = false;
	}

	for (auto button : m_buttons)
	{
		button->update();
	}

	m_highlighter.update();
}

void Menu::activatePrevButton()
{
	m_buttons[m_activeButtonIndex]->deactivate();
	m_activeButtonIndex--;
	if (m_activeButtonIndex < 0)
		m_activeButtonIndex = m_buttons.size() - 1;
	m_buttons[m_activeButtonIndex]->activate();

	setHighlighter(m_activeButtonIndex);

	playSound(OBJ_TYPE::MENU_TYPE, menuSoundTypes::MENU_SOUND_TYPE_NEXT_BUTTON);
}

void Menu::activateNextButton()
{
	m_buttons[m_activeButtonIndex]->deactivate();
	m_activeButtonIndex++;
	if (m_activeButtonIndex >= m_buttons.size())
		m_activeButtonIndex = 0;
	m_buttons[m_activeButtonIndex]->activate();

	setHighlighter(m_activeButtonIndex);

	playSound(OBJ_TYPE::MENU_TYPE, menuSoundTypes::MENU_SOUND_TYPE_NEXT_BUTTON);
}

void Menu::draw(sf::RenderWindow &window)
{
	GameObject::draw(window);

	for (auto button : m_buttons)
	{
		button->draw(window);
	}

	m_highlighter.draw(window);
}

void Menu::playAudio()
{
	playBackgroundMusic();
}

void Menu::playBackgroundMusic()
{
	BackgroundAudioPlayer::getInstance()->play(m_backgroundMusicFilename);
}

void Menu::loadContent()
{
	GameObject::loadContent();
	for (auto button : m_buttons)
	{
		button->loadContent();
	}

	m_highlighter.loadContent();

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

void Menu::setAndInsertButtons(const std::vector<BUTTON_TYPE>& buttonTypes)
{
	if (buttonTypes.size() > 0)
	{
		const double scale = 0.7;
		const double w = m_rect.w * scale;
		const double x = (m_rect.w - w) / 2;
		const size_t buttonsNum = buttonTypes.size();
		const int allSpaces = 2 * buttonsNum + 1;
		double h = m_rect.h / allSpaces;
		double y = h;
		for (size_t i = 0; i < buttonsNum; ++i)
		{
			if ((buttonTypes[i] == BUTTON_TYPE::SAVE_SLOT) ||
				(buttonTypes[i] == BUTTON_TYPE::LOAD_SLOT))
			{
				h = 1.5 * m_rect.h / allSpaces;
				m_buttons.push_back(new SaveSlotButton(x, y, w, h, false, i + 1, buttonTypes[i]));
				y += h;
			}
			else
			{
				m_buttons.push_back(new MenuButton(x, y, w, h, false, buttonTypes[i]));
				y += 2 * h;
			}
		}
	}
}

void Menu::setHighlighter(size_t buttonIndex)
{
	if (m_buttons.size() > 0)
	{
		const double scale = 1.05;
		const Rectangle buttonRect = m_buttons[buttonIndex]->getRect();
		const double w = buttonRect.w * scale;
		const double h = buttonRect.h * scale;
		const double x = buttonRect.x - (w - buttonRect.w) / 2;
		const double y = buttonRect.y - (h - buttonRect.h) / 2;
		m_highlighter.setRect(Rectangle(x, y, w, h));
	}
}