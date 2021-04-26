#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <unordered_map>
#include <vector>
#include <fstream>				// read from file "levels" which textures to load
#include <string>				// strings manipulations while reading data 
#include <sstream>				// a string line read from file to be converted to ints

#include <SFML\Graphics.hpp>	// sf::Texture, sf::Sprite

#include "Definitions.h"		// ObjectType, Animations

typedef UMAP<OBJ_TYPE, std::string> umapTypeString;
typedef UMAP<OBJ_TYPE, std::vector<std::string> > umapTypeVecStrings;
typedef UMAP<OBJ_TYPE, std::vector<size_t> > umapTypeVecInts;
struct SoundBuffersHolder
{
	std::vector<sf::SoundBuffer> soundBuffers;
		// specifies the ranges of different sounds for speciffic type, i.e. if ranges[0] == 5 says 
		// that soundBuffers[0..4] are of type 0. Then if ranges[1] == 3 then 
		// soundBuffers[ranges[0]..ranges[0] + 3] == ranges[5..7] are of type 1, etc.
	std::vector<size_t> ranges; 
};

// fwd declarations
class GameObject;
class Menu;

class ResourcesManager
{
private:
	const double m_speedFactroDivider = 100;
public:
	static ResourcesManager* getInstance();
	void		initialize();
	void		loadResources(unsigned int level);
	sf::Texture getTexture(OBJ_TYPE command);
	bool        getAnimation(OBJ_TYPE command, Animation& animation);
	SoundBuffersHolder& getSoundBuffers(OBJ_TYPE type, bool& noBuffers);
	void		setWindowDimensions(double w, double h);
	void		getWindowDimensions(double& w, double& h) const { w = m_windowDimensions.w; h = m_windowDimensions.h; }
	std::vector<GameObject *>& getGameObjects() { return m_gameObjects; }
	void		addGameObject(GameObject* gameObject);
	void		removeInactiveGameObjects();
	UMAP<RUN_MENU_STATE, Menu *>& getMenus() { return m_menus; }
	const sf::Vector2f getSpeedFactor() const { return m_speedFactor; }
	const sf::Vector2f getLevelBlockDimensions();

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
					umapTypeString& imagesNames,
					std::vector<OBJ_TYPE>& resCommands,
					umapTypeVecStrings& soundsNames,
					umapTypeVecInts& soundsRanges);
	void loadMenus(umapTypeString& imagesNames, umapTypeString& musicNames);
	void initMenus(std::vector<OBJ_TYPE>& resCommands, umapTypeString& musicNames);
	std::ifstream getReader(std::string filePath);
	void getZeroInfo(umapTypeString& imagesNames, 
					 umapTypeVecStrings& soundsNames,
					 umapTypeVecInts& soundsRanges,
					 double& zeroSpeed, 
					 double& zeroHealth, 
					 double& zeroAttcackingSpeed,
					 double& zeroFiringAccurracy);
	void getBackgroundInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getWallsInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getMonstersInfo(const unsigned int& level, 
						 MONSTERS_TYPE& monsterType, 
						 umapTypeString& imagesNames, 
						 umapTypeVecStrings& soundsNames,
						 umapTypeVecInts& soundsRanges,
						 double& damage, 
						 double& speed,
						 double& health,
						 double& attackingSpeed,
						 std::vector<OBJ_TYPE>& monsterImmuneFrom);
	void getWeaponsInfo(const unsigned int& level, 
						std::vector<OBJ_TYPE>& weaponType,
						std::vector<OBJ_TYPE>& projectilesType,
						umapTypeString& imagesNames,
						umapTypeVecStrings& soundsNames,
						umapTypeVecInts& soundsRanges,
						double& projectilesDamage,
						double& firingRate,
						double& projectilesSpeed);
	void getEndOfLevelInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getHealthInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getWeaponInfo(const unsigned int& level, UMAP<OBJ_TYPE, std::string>& imagesNames);
	void getGeneralInfo(const unsigned int& level, 
						unsigned int& numbersOfLevels, std::vector<sf::Vector2u>& wallsCoords,
						sf::Vector2u& endOfLevelCoords,
						std::vector<sf::Vector2u>& monstersCoords,
						std::vector<sf::Vector2u>& weaponsCoords);
	void setSpeedFactor();
	void readFromFileSoundsFileNames(std::ifstream& fileReader, const OBJ_TYPE& gameObjectType, umapTypeVecStrings& soundsNames, umapTypeVecInts& soundsRanges);

	// members
	static ResourcesManager* m_instance;
	UMAP<OBJ_TYPE, sf::Texture> m_textures;
	UMAP<OBJ_TYPE, Animation> m_animations;
	UMAP<OBJ_TYPE, SoundBuffersHolder > m_soundBuffers;
	Rectangle m_windowDimensions;
	sf::Vector2u m_objectsInLevel; // x = how many columns, y = how many rows current level has
	enum objTypeOnLevelMap
	{
		MAP_EMPTY_OBJ = 0,
		MAP_WALL_OBJ,
		MAP_MONSTER_OBJ,
		MAP_WEAPON_OBJ,
		MAP_END_OF_LEVEL_OBJ = 9
	};

	std::vector<GameObject *> m_gameObjects;
	UMAP<RUN_MENU_STATE, Menu *> m_menus;
	sf::Vector2f m_speedFactor;
};

#endif