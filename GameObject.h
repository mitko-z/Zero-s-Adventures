#pragma once

#include <unordered_map>	// map of controlling keys
#include <vector>			// set of textures

#include <SFML/Graphics.hpp>	// Keyboard::Key type

#include "Definitions.h"		// ObjectType
#include "ResourcesManager.h"	// get textures from the resources manager

#define MAP_KEYS UMAP<sf::Keyboard::Key, bool>

class GameObject
{
public :
	GameObject() = default;
	GameObject(double x, double y, double w, double h, bool isAnimating) : 
		rect(x, y, w, h),
		isAnimating(isAnimating)
	{
		hasCollisions = false;
	}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual Definitions::ObjectType getLoadResourcesCommand();
	virtual void updateEvents();
	virtual void nullCollisions();
	virtual void setCollisionWith(GameObject& other);
	virtual void processCollisions() {}
	virtual void setRect(Rectangle rect) { this->rect = rect; }
	virtual const Rectangle getRect() const { return this->rect; }
	virtual void update();
	virtual void draw(sf::RenderWindow &window);
	virtual ~GameObject() = 0;	// abstract class - cannot instanciate
protected :
	virtual void updateDrawingObject();
	virtual void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased);

	Rectangle rect;
	DrawingObject drawingObject;
	std::map<sf::Keyboard::Key, bool> controllingKeys;
	Animation frames;
	bool isAnimating;
	int animationFrame;
	bool hasCollisions;
	std::vector<GameObject*> objsColideWith;
private:
	void scaleSpriteTo(double w, double h, const sf::Texture& texture, sf::Sprite& sprite);
	void updateAnimFrame();
};