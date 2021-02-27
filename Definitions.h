#pragma once

#include <SFML\Graphics.hpp>

#define OBJ_TYPE Definitions::ObjectType
#define MODE Definitions::Mode
#define RUN_GAME_STATE Definitions::RunningGameState
#define RUN_MENU_STATE Definitions::RunningMenuState
#define BUTTON_TYPE Definitions::ButtonType
#define COMMAND Definitions::Command
#define MONSTERS_TYPE Definitions::MonstersType

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
		EXIT_MODE,
		INITIALIZE_MODE
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
		FINISHED_LEVEL_SCREEN_STATE,
		GAME_OVER_SCREEN_STATE,
		FINAL_SCREEN_STATE
	};

	enum ObjectType
	{
		NO_OBJ_TYPE = 0,
		BACKGROUND_TYPE = 1,
		ZERO_TYPE = 2,
		MENU_BUTTON_TYPE = 3,
		MENU_TYPE = 4,
		BUTTON_HIGHLIGHTER_TYPE = 5,
		START_SCREEN_TYPE = 6,
		WALL_TYPE = 7,
		END_OF_LEVEL_TYPE = 8,
		FINISHED_LEVEL_SCREEN_TYPE = 9,
		MONSTER_TYPE = 10,
		HEALTH_TYPE = 11,
		HEALTH_BACKGROUND_TYPE = 12,
		WEAPONS_TYPES_START = 13,
		BOW_WEAPON_TYPE = 14,
		FIREBALL_WEAPON_TYPE = 15,
		STAR_WEAPON_TYPE = 16,
		PROJECTILES_TYPES_START = 17,
		ARROW_PROJECTILE_TYPE = 18,
		FIREBALL_PROJECTILE_TYPE = 19,
		STAR_PROJECTILE_TYPE = 20,
		GAME_OVER_SCREEN_TYPE = 21,
		FINAL_SCREEN = 22,
	};

	enum MonstersType
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
		GAME_COMMAND,					// going to game mode
		MENU_COMMAND,					// to menu mode
		EXIT_COMMAND,					// exit the game
		MIAN_MENU_COMMAND,				// show the main menu
		FINISHED_LEVEL_SCREEN_COMMAND,	// show the finished level screen
		NEXT_LEVEL_COMMAND,				// go to the next level
		FINAL_SCREEN_COMMAND,			// show the final screen
		GAME_OVER_COMMAND,				// show the game over screen
		START_SCREEN_COMMAND			// initialize & show the start screen		
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