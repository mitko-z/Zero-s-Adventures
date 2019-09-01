#pragma once

#include <unordered_map>	// map of controlling keys
#include <vector>			// set of textures

#include <SFML/Graphics.hpp>	// Keyboard::Key type

#include "Definitions.h"		// LoadResourcesCommands
#include "ResourcesManager.h"	// get textures from the resources manager


class GameObject
{
public :
	GameObject() = default;
	GameObject(double x, double y, double w, double h) : rect(x, y, w, y) {}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual LoadResourcesCommands getLoadResourcesCommand();
	virtual void updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &keysPressed);
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

	struct DrawingObject
	{
		sf::Texture texture;
		sf::RectangleShape rectangleShape;
	};
	DrawingObject drawingObject;
	void updateDrawingObject();

	std::map<sf::Keyboard::Key, bool> controllingKeys;
};