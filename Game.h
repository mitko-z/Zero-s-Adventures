#pragma once
#include <string>
#include <unordered_map>
#include <iostream>		// std::cerr
#include <memory>		// unique_ptr-s for game objects
#include <vector>		// vector of game objects

#include "SFML\Graphics.hpp"

#include "ResourcesManager.h"
#include "Definitions.h"
#include "GameObject.h"
#include "Background.h"
#include "ZeroCharacter.h"

class Game
{
public:

#pragma region constructor(s)
	Game();
#pragma endregion

#pragma region methods
	/// <summary>
	/// run() - start & handle the game loop
	/// </summary>
	void run();

private:
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
	/// eventsCapture() - get events from outside world
	/// </summary>
	void eventsCapture();

	/// <summary>
	/// update() Allows the game to run logic such as updating the world,
	/// checking for collisions, gathering input, and playing audio.
	/// </summary>
	void update();

	/// <summary>
	/// This is called when the game should draw itself.
	/// </summary>
	void draw();
#pragma endregion

#pragma region members
	// windows
	sf::RenderWindow window;

	// display text on screen variables
	sf::Font font;
	std::vector<std::string> finishedLevelStrings;

	// states
	Mode mode;
	RunningGameStates runningGameState;
	RunningMenuStates runningMenuState;
	RunningMenuStates previousRunningMenuState;

	// time management
	// difne how many miliseconds to show the screen between the levels before to advance to the next level
	// the pause is necessary to avoid any keys pressed in the previous level
	const int timePauseBetweenLevels;
	// how miliseconds left before checking whether any key is pressed to advance to the next level
	int timeElapsedBetweenLevels;

	// events
	sf::Event event;
	std::unordered_map<sf::Keyboard::Key, bool> keysPressed;

	// game objects
	std::vector<GameObject *> gameObjects;

#pragma endregion

};
