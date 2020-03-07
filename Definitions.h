#pragma once

#include <SFML\Graphics.hpp>

#define LOAD_RES_COMMAND Definitions::LoadResourcesCommands
#define UMAP std::unordered_map

namespace Definitions
{
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
		Menu,
		SaveGameMenu,
		LoadGameMenu,
		OptionsMenu
	};

	enum LoadResourcesCommands
	{
		NONE = 0,
		BACKGROUND,
		ZERO,
		MENU_BUTTON,
		MAIN_MENU,
		BUTTON_HIGHLIGHTER
	};
}

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
	Animation() : framesAlongX(1), framesAlongY(1) {}
	Animation(int x, int y) : framesAlongX(x), framesAlongY(y) {}
	int framesAlongX;
	int framesAlongY;
};