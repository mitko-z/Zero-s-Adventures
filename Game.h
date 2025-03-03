#pragma once

#include <string>

#include "SFML\Graphics.hpp"

class GameObject;


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

#pragma region private methods
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

	/// <summary>
	/// Save the state of the game to a slot defined in the state machine
	/// </summary>
	void saveGame();

	/// <summary>
	/// Load the state of the game from a slot defined in the state machine
	/// </summary>
	void loadGame();

	std::string getCurrentDateTime();
	void appendToOSSWithNewline(std::ostringstream& oss, const std::string& data);
#pragma endregion

#pragma region members
	// windows
	sf::RenderWindow m_window;

	// events
	sf::Event m_event;

	unsigned int m_currentLevel;

#pragma endregion

};
