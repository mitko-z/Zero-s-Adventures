#ifndef GAME_H
#define GAME_H

#pragma once
#include <string>
#include <unordered_map>
#include <iostream>		// std::cerr
#include <memory>		// unique_ptr-s for game objects
#include <vector>		// vector of game objects

#include "SFML\Graphics.hpp"

#include "GameObject.h"
#include "Menu.h"

class Game
{
public:

#pragma region public methods
	Game();

	~Game();

	/// <summary>
	/// initialize() - allows the game to perform any initialization it needs to before starting to run.
	/// This is where it can query for any required services and load any non-graphic
	/// related content.
	/// </summary>
	void initialize();

	/// <summary>
	/// loadContent() - load data
	/// </summary>
	void loadContent();

	/// <summary>
	/// run() - start & handle the game loop
	/// </summary>
	void run();
#pragma endregion

private:

#pragma region public methods
	/// <summary>
	/// eventsCapture() - capture and process events such as keyboard 
	/// pressed, collisions between objects, timers, etc.
	/// </summary>
	void eventsCapture();

	void processColisions();
	bool twoObjsColide(const GameObject& obj1, const GameObject& obj2);

	/// <summary>
	/// update() Allows the game to run logic
	/// </summary>
	void update();

	/// <summary>
	/// This is called when the game should draw on screen objects
	/// </summary>
	void draw();

	/// <summary>
	/// playing sounds from various objects
	/// </summary>
	void playAudio();
#pragma endregion

#pragma region members
	// windows
	sf::RenderWindow m_window;

	// events
	sf::Event m_event;

	unsigned int m_currentLevel;

#pragma endregion

};

#endif