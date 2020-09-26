#pragma once

#include <SFML\Graphics.hpp>

#define OBJ_TYPE Definitions::ObjectType
#define MODE Definitions::Mode
#define RUN_GAME_STATE Definitions::RunningGameState
#define RUN_MENU_STATE Definitions::RunningMenuState
#define BUTTON_TYPE Definitions::ButtonType
#define COMMAND Definitions::Command
#define MONSTER_TYPES Definitions::MonsterTypes

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
		NEXT_LEVEL_MODE,
		MENU_MODE,
		EXIT_MODE
	};

	/// <summary>
	/// Define the states of the game in game mode
	/// </summary>
	enum RunningGameState
	{
		PLAYING_STATE
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
		FINISHED_LEVEL_SCREEN_STATE
	};

	enum ObjectType
	{
		NO_OBJ_TYPE = 0,
		BACKGROUND_TYPE,
		ZERO_TYPE,
		MENU_BUTTON_TYPE,
		MENU_TYPE,
		BUTTON_HIGHLIGHTER_TYPE,
		START_SCREEN_TYPE,
		WALL_TYPE,
		END_OF_LEVEL_TYPE,
		FINISHED_LEVEL_SCREEN_TYPE,
		MONSTER_TYPE,
		HEALTH_TYPE,
		HEALTH_BACKGROUND_TYPE
	};

	enum MonsterTypes
	{
		NO_MONSTER_TYPE = 0,
		JELLY_MONSTER_TYPE,
		ONE_EYE_MONSTER_TYPE,
		WALKING_SQUARE_MONSTER_TYPE
	};

	enum ButtonType
	{
		START_GAME_BUTTON,
		EXIT_GAME_BUTTON
	};

	enum Command
	{
		GAME_COMMAND,					// command for going to game mode
		MENU_COMMAND,					// command for going to menu mode
		EXIT_COMMAND,					// command for exiting the game
		MIAN_MENU_COMMAND,				// command for showing the main menu
		FINISHED_LEVEL_SCREEN_COMMAND,	// command for showing the finished level screen
		NEXT_LEVEL_COMMAND,				// command for going to the next level
		FINAL_SCREEN_COMMAND			// command for showing the final screen
	};
}

struct Rectangle
{
	double x, y, w, h;
	Rectangle() = default;
	Rectangle(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {}

	double center_x() { return x + (w / 2); }
	double center_y() { return y + (h / 2); }
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