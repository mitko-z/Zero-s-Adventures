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
		m_isActive = true;
	}
	GameObject(GameObject &other);
	virtual void initialize();
	virtual void loadContent();
	virtual OBJ_TYPE getType();
	virtual void updateEvents();
	virtual void nullCollisions();
	virtual void setCollisionWith(GameObject& other);
	virtual void processCollisions() {}
	virtual void setRect(Rectangle rect) { m_rect = rect; }
	virtual const Rectangle getRect() const { return m_rect; }
	virtual void update();
	virtual void draw(sf::RenderWindow &window);
	virtual void playAudio() {}
	virtual const bool	isActive() const { return m_isActive; }
	virtual ~GameObject() = 0;	// abstract class - cannot instanciate
	virtual std::ostringstream getCurrentState();
protected :
	virtual void updateDrawingObject();
	virtual void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased);
	void scaleSpriteTo(double w, double h, const sf::Texture& texture, sf::Sprite& sprite);
	void playSound(OBJ_TYPE gameObjType, int rangeType);
	std::string addLineForOSS(std::string data, bool addComment = false, std::string comment = "");

	// graphics
	Rectangle m_rect;
	DrawingObject m_drawingObject;
	std::map<sf::Keyboard::Key, bool> m_controllingKeys;
	Animation m_frames;
	bool m_isAnimating;
	int m_animationFrame;

	// collisions
	bool m_hasCollisions;
	std::vector<GameObject*> m_objsColideWith;

	// audio
	SoundObject m_soundObject;
	std::string m_pathToMusicBackground;

	bool m_isActive;
private:
	void updateAnimFrame();
};