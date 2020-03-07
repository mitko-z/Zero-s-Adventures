#include "EventsHolder.h"

void EventsHolder::addPressedKey(sf::Keyboard::Key key)
{
	keysPressed[key] = true;
}

void EventsHolder::addReleasedKey(sf::Keyboard::Key key)
{
	keysReleased[key] = true;
}

void EventsHolder::nullEvents()
{
	for (auto it = keysPressed.begin(); it != keysPressed.end(); ++it)
	{
		it->second = false;
	}
	for (auto it = keysReleased.begin(); it != keysReleased.end(); ++it)
	{
		it->second = false;
	}
}