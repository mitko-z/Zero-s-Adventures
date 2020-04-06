#pragma once

#include <SFML\Graphics.hpp>

#define LOAD_RES_COMMAND Definitions::LoadResourcesCommand
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
		GAME_MODE,
		MENU_MODE,
		EXIT_MODE
	};

	/// <summary>
	/// Define the states of the game in game mode
	/// </summary>
	enum RunningGameState
	{
		PLAYING_STATE,
		FINISHED_LEVEL_SCREEN_STATE,
		GAME_FINISHED_SCREEN_STATE,
		GAME_OVER_SCRREN_STATE
	};

	/// <summary>
	/// Define the states of the game in menu mode
	/// </summary>
	enum RunningMenuState
	{
		MAIN_MENU_STATE,
		RESUME_MENU_STATE,
		SAVE_GAME_STATE,
		LOAD_GAME_STATE,
		OPTIONS_STATE,
		START_SCREEN_STATE,
	};

	enum LoadResourcesCommand
	{
		NONE = 0,
		BACKGROUND_RES,
		ZERO_RES,
		MENU_BUTTON_RES,
		MENU_RES,
		BUTTON_HIGHLIGHTER_RES,
		START_SCREEN_RES
	};

	enum ButtonType
	{
		START_GAME_BUTTON,
		EXIT_GAME_BUTTON
	};

	enum GameCommand
	{
		GAME_COMMAND,
		MENU_COMMAND,
		EXIT_COMMAND,
		MIAN_MENU_COMMAND
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