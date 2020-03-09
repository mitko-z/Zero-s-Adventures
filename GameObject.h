#pragma once

#include <unordered_map>	// map of controlling keys
#include <vector>			// set of textures

#include <SFML/Graphics.hpp>	// Keyboard::Key type

#include "Definitions.h"		// LoadResourcesCommand
#include "ResourcesManager.h"	// get textures from the resources manager


class GameObject
{
public :
	GameObject() = default;
	GameObject(double x, double y, double w, double h, bool isAnimating) : 
		rect(x, y, w, h),
		isAnimating(isAnimating)
	{}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual Definitions::LoadResourcesCommand getLoadResourcesCommand();
	virtual void updateEvents();
	virtual void setRect(Rectangle rect) { this->rect = rect; }
	virtual const Rectangle getRect() const { return this->rect; }
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
	void updateKeys(const UMAP<sf::Keyboard::Key, bool>& keys);
	void scaleSpriteTo(double w, double h, const sf::Texture& texture, sf::Sprite& sprite);
private:
	void updateAnimFrame();
};