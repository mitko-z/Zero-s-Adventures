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

#pragma region constructor(s)
	Game();
#pragma endregion

#pragma region public methods

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
	/// This is called when the game should draw on screen objects (also playing sounds)
	/// </summary>
	void draw();
#pragma endregion

#pragma region members
	// windows
	sf::RenderWindow window;

	// display text on screen variables
	sf::Font font;
	std::vector<std::string> finishedLevelStrings;

	// time management
	// difne how many miliseconds to show the screen between the levels before to advance to the next level
	// the pause is necessary to avoid any keys pressed in the previous level
	const int timePauseBetweenLevels;
	// how miliseconds left before checking whether any key is pressed to advance to the next level
	int timeElapsedBetweenLevels;

	// events
	sf::Event event;

#pragma endregion

};

#endif