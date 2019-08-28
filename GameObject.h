#pragma once

#include <unordered_map>	// map of controlling keys
#include <vector>			// set of textures

#include <SFML/Graphics.hpp>	// Keyboard::Key type


class GameObject
{
public :
	GameObject() = default;
	GameObject(int x, int y, int w, int h) : rect(x, y, w, y) {}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual void updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &controllingKeys);
	virtual void update();
	virtual void draw(sf::RenderWindow &window);
	virtual ~GameObject() = 0;	// abstract class - cannot instanciate
protected :
	struct Rectangle
	{
		double x, y, w, h;
		Rectangle() = default;
		Rectangle(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {}
	};
	Rectangle rect;

	sf::Texture texture;
	sf::RectangleShape drawingObject;

	std::map<sf::Keyboard::Key, bool> controllingKeys;
};