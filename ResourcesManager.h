#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <unordered_map>
#include <vector>
#include <fstream>				// read from file "levels" which textures to load
#include <string>				// strings manipulations while reading data 
#include <sstream>				// a string line read from file to be converted to ints

#include <SFML\Graphics.hpp>	// sf::Texture, sf::Sprite

#include "Definitions.h"		// ObjectType, Animations

//#include "GameObject.h"
//#include "Menu.h"

// fwd declarations
class GameObject;
class Menu;

class ResourcesManager
{
public:
	static ResourcesManager* getInstance();
	void		loadResources(unsigned int level);
	sf::Texture getTexture(Definitions::ObjectType command);
	bool        getAnimation(Definitions::ObjectType command, Animation& animation);
	void		setWindowDimensions(float w, float h);
	void		getWindowDimensions(float& w, float& h) const { w = m_windowDimensions.w; h = m_windowDimensions.h; }
	std::vector<GameObject *>& getGameObjects() { return m_gameObjects; }
	UMAP<RUN_MENU_STATE, Menu *>& getMenus() { return m_menus; }

	~ResourcesManager();
private:
	// private methods
	ResourcesManager() {};
	ResourcesManager(ResourcesManager const&);
	void operator= (ResourcesManager const&);
	Animation getAnimationFromString(std::string strData);
	sf::Vector2u getGameObjSize();
	sf::Vector2u calcWorldCoordsFromMapCoords(const sf::Vector2u& mapCoords);
	void loadLevel(unsigned int level,
		UMAP<OBJ_TYPE, std::string>& imagesNames,
		std::vector<OBJ_TYPE>& resCommands);
	std::ifstream getReader(std::string filePath);
	void getZeroInfo(UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getBackgroundInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getWallsInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getMonstersInfo(const unsigned int& level, MONSTER_TYPES& monsterType, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getWeaponsInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getEndOfLevelInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getGeneralInfo(const unsigned int& level, 
						unsigned int& numbersOfLevels, std::vector<sf::Vector2u>& wallsCoords,
						sf::Vector2u& endOfLevelCoords,
						std::vector<sf::Vector2u>& monstersCoords);
	// members
	static ResourcesManager* m_instance;
	UMAP<OBJ_TYPE, sf::Texture> m_textures;
	UMAP<OBJ_TYPE, Animation> m_animations;
	Rectangle m_windowDimensions;
	sf::Vector2u m_objectsInLevel; // x = how many columns, y = how many rows current level has
	enum objTypeOnLevelMap
	{
		EMPTY = 0,
		WALL,
		MONSTER,
		WEAPON,
		END_OF_LEVEL = 9
	};

	std::vector<GameObject *> m_gameObjects;
	UMAP<RUN_MENU_STATE, Menu *> m_menus;
};

#endif