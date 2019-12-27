#pragma once


/// <summary>
/// define two modes of this game
/// GameMode - the game plays and shows screens between levels
/// MenuMode - the game is paused and menus are managed
/// </summary>
enum Mode
{
	gameMode,
	menuMode,
	exitMode
};

/// <summary>
/// Define the states of the game in game mode
/// </summary>
enum RunningGameStates
{
	StartScreen,
	Playing,
	FinishedLevelScreen,
	GameFinished,
	GameOver
};

/// <summary>
/// Define the states of the game in menu mode
/// </summary>
enum RunningMenuStates
{
	MainMenu,
	SaveGameMenu,
	LoadGameMenu,
	OptionsMenu
};

enum LoadResourcesCommands
{
	NONE		= 0,
	BACKGROUND,
	ZERO
};


struct Rectangle
{
	double x, y, w, h;
	Rectangle() = default;
	Rectangle(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {}
};


struct DrawingObject
{
	sf::Texture texture;
	//sf::RectangleShape rectangleShape;
	sf::Sprite sprite;
};


struct Animation
{
public:
	int framesAlongX;
	int framesAlongY;
};