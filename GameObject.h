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
	GameObject(double x, double y, double w, double h, bool isAnimating) : 
		rect(x, y, w, y),
		isAnimating(isAnimating)
	{}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual LoadResourcesCommands getLoadResourcesCommand();
	virtual void updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &keysPressed);
	virtual void update();
	virtual void draw(sf::RenderWindow &window);
	virtual ~GameObject() = 0;	// abstract class - cannot instanciate
protected :
	Rectangle rect;
	DrawingObject drawingObject;
	std::map<sf::Keyboard::Key, bool> controllingKeys;
	Animation frames;
	bool isAnimating;
	static int animationFrame;

	void updateDrawingObject();
private:
	void updateAnimFrame();
};