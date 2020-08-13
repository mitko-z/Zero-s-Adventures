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
		m_rect(x, y, w, h),
		m_isAnimating(isAnimating)
	{
		m_hasCollisions = false;
	}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual Definitions::ObjectType getLoadResourcesCommand();
	virtual void updateEvents();
	virtual void nullCollisions();
	virtual void setCollisionWith(GameObject& other);
	virtual void processCollisions() {}
	virtual void setRect(Rectangle rect) { this->m_rect = rect; }
	virtual const Rectangle getRect() const { return this->m_rect; }
	virtual void update();
	virtual void draw(sf::RenderWindow &window);
	virtual ~GameObject() = 0;	// abstract class - cannot instanciate
protected :
	virtual void updateDrawingObject();
	virtual void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased);

	Rectangle m_rect;
	DrawingObject m_drawingObject;
	std::map<sf::Keyboard::Key, bool> m_controllingKeys;
	Animation m_frames;
	bool m_isAnimating;
	int m_animationFrame;
	bool m_hasCollisions;
	std::vector<GameObject*> m_objsColideWith;
private:
	void scaleSpriteTo(double w, double h, const sf::Texture& texture, sf::Sprite& sprite);
	void updateAnimFrame();
};