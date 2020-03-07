#pragma once

#include <unordered_map>

#include <SFML\Window.hpp>

#include "Definitions.h"

class EventsHolder
{
public:
	const UMAP<sf::Keyboard::Key, bool>& getPressedKeys() { return keysPressed; }
	void addPressedKey(sf::Keyboard::Key key);
	const UMAP<sf::Keyboard::Key, bool>& getReleasedKeys() { return keysReleased; }
	void addReleasedKey(sf::Keyboard::Key key);
	void nullEvents();
private:
	UMAP<sf::Keyboard::Key, bool> keysPressed;
	UMAP<sf::Keyboard::Key, bool> keysReleased;
};