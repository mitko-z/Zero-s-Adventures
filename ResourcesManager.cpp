#include "ResourcesManager.h"

#include <sstream>

#include "ZeroCharacter.h"
#include "Background.h"
#include "MainMenu.h"
#include "ResumeGameMenu.h"
#include "SaveMenu.h"
#include "LoadMenu.h"
#include "StartScreen.h"
#include "Wall.h"
#include "EndOfLevel.h"
#include "FinishedLevelScreen.h"
#include "GameOverScreen.h"
#include "FinalScreen.h"
#include "Monster.h"
#include "Health.h"
#include "Weapon.h"
#include "FileReadWriteTools.h"
#include "LevelHeader.h"

// file reading defines
#define GENERAL_INFO_FILE_PATH		"Data/general_info.dat"
#define WALLS_INFO_FILE_PATH		"Data/walls_info.dat"
#define BACKGROUND_INFO_FILE_PATH	"Data/background_info.dat"
#define ZERO_INFO_FILE_PATH			"Data/zero_info.dat"
#define MONSTERS_INFO_FILE_PATH		"Data/monsters_info.dat"
#define WEAPONS_INFO_FILE_PATH		"Data/weapons_info.dat"
#define END_OF_LEVEL_INFO_FILE_PATH "Data/end_of_level_info.dat"
#define HEALTH_INFO_FILE_PATH		"Data/health_info.dat"
#define TIMER_LEVEL_FILE_PATH		"Data/timerLevel_info.dat"
#define MENUS_INFO_FILE_PATH		"Data/menus_info.dat"
#define IMAGES_FOLDER				"Data/Images/"
#define SOUNDS_FOLDER				"Data/Audio/sounds/"
#define AUDIO_FOLDER				"Data/Audio/"

// TODO - move these below to a settings file (when one is available)
#define START_SCREEN_SECONDS_TO_WAIT			0
#define FINISHED_LEVEL_SCREEN_SECONDS_TO_WAIT	2
#define GAME_OVER_SCREEN_SECONDS_TO_WAIT		5

std::shared_ptr <ResourcesManager> ResourcesManager::getInstance()
{
	static std::shared_ptr<ResourcesManager> instance{ new ResourcesManager };
	return instance;
}

void ResourcesManager::initialize()
{
	m_gameObjects.clear();
	m_menus.clear();
	m_textures.clear();
	m_animations.clear();
}

void ResourcesManager::setWindowDimensions(double w, double h)
{
	m_windowDimensions.w = w;
	m_windowDimensions.h = h;
}

void ResourcesManager::addGameObject(GameObject* gameObject)
{
	m_gameObjects.push_back(gameObject);
}

void ResourcesManager::removeInactiveGameObjects()
{
	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		if (!m_gameObjects[i]->isActive())
			m_gameObjects.erase(m_gameObjects.begin() + i);
	}
}

void ResourcesManager::loadResources(unsigned int level)
{
	umapTypeString imagesNames;
	std::vector<OBJ_TYPE> resourcesTypes;
	umapTypeVecStrings soundNames;
	umapTypeVecInts soundsRanges;
	umapTypeString musicBackgroundNames;

	loadLevel(level, imagesNames, resourcesTypes, soundNames, soundsRanges);

	// Menu load
	if (level == 1)
	{
		loadMenus(imagesNames, musicBackgroundNames, soundNames, soundsRanges);
		initMenus(resourcesTypes, musicBackgroundNames);
	}

	for (auto resType : resourcesTypes)
	{
		sf::Texture texture;
		std::string loadPath(IMAGES_FOLDER);
		loadPath += imagesNames[resType];
		if (!texture.loadFromFile(loadPath))
		{
			std::string throwMessage = "Cannot load image " + loadPath;
			throw throwMessage;
		}
		m_textures[resType] = std::make_pair(loadPath, texture);
	} // end for each resType

	for (auto& typeSoundsNames : soundNames)
	{
		for (auto& soundName : typeSoundsNames.second)
		{
			sf::SoundBuffer buf;
			if (!buf.loadFromFile(SOUNDS_FOLDER + soundName))
			{
				std::string throwMessage = "Cannot load sound " + soundName;
				throw throwMessage;
			}

			m_soundBuffers[typeSoundsNames.first].soundBuffers.push_back(buf);
		}
	}

	for (auto& typeSoundsRanges : soundsRanges)
	{
		for (auto& soundRange : typeSoundsRanges.second)
		{
			m_soundBuffers[typeSoundsRanges.first].ranges.push_back(soundRange);
		}
	}
}

Animation ResourcesManager::getAnimationFromString(const std::string& strData)
{
	int x, y;
	std::stringstream ss;
	ss << strData;
	std::string temp;
	ss >> temp;
	std::stringstream(temp) >> x;
	ss >> temp;
	std::stringstream(temp) >> y;

	return Animation( x,y );
}

sf::Vector2u ResourcesManager::getGameObjSize()
{
	return 
		sf::Vector2u(
			(m_windowDimensions.w / m_objectsInLevel.x), 
			(m_windowDimensions.h / m_objectsInLevel.y));
}

sf::Vector2u ResourcesManager::calcWorldCoordsFromMapCoords(const sf::Vector2u& mapCoords)
{
	sf::Vector2u sizeOfObj(sf::Vector2u((m_windowDimensions.w / m_objectsInLevel.x), (m_windowDimensions.h / m_objectsInLevel.y)));

	sf::Vector2u result(sf::Vector2u((sizeOfObj.x * mapCoords.x), (sizeOfObj.y * mapCoords.y)));

	return result;
}

void ResourcesManager::loadLevel(unsigned int level,
								 umapTypeString& imagesNames,
								 std::vector<OBJ_TYPE>& resourcesTypes,
								 umapTypeVecStrings& soundsNames,
								 umapTypeVecInts& soundsRanges)
{
	// get Zero's previous stats, if any and if we are not on level one
	Weapon* ZeroCurrentWeapon = nullptr;
	if (m_gameObjects.size() >= 1 && level > 1)
	{
		ZeroCurrentWeapon = dynamic_cast<ZeroCharacter*>(m_gameObjects[1])->getCurrentWeapon();
	}

	// clear all objects so they can be loaded freshly
	m_gameObjects.clear();
	imagesNames.clear();
	resourcesTypes.clear();
	soundsNames.clear();
	m_animations.clear();

	/// reading infos
	// read zero info
	double zeroSpeed, zeroHealth, zeroAttcackingSpeed, zeroFiringAccurracy;
	getZeroInfo(imagesNames, soundsNames, soundsRanges, zeroSpeed, zeroHealth, zeroAttcackingSpeed, zeroFiringAccurracy);

	// read background info
	std::string backgroundMusicFileName;
	getBackgroundInfo(level, imagesNames, backgroundMusicFileName);

	// read walls info
	getWallsInfo(level, imagesNames);

	// read monsters info
	MONSTERS_TYPE monsterType = MONSTERS_TYPE::NO_MONSTER_TYPE;
	double monsterDamage, monsterSpeed, monsterHealth, monsterAttackingSpeed;
	std::vector<OBJ_TYPE> monsterImmuneFrom;
	getMonstersInfo(level, monsterType, imagesNames, soundsNames, soundsRanges, monsterDamage, monsterSpeed, monsterHealth, monsterAttackingSpeed, monsterImmuneFrom);

	// read wapons & projectiles info
	std::vector<OBJ_TYPE> allWeaponsTypes;
	std::vector<OBJ_TYPE> allProjectilesTypes;
	double weaponAttackSpeed, projectilesDamage, projectilesSpeed;
	getWeaponsInfo(level, allWeaponsTypes, allProjectilesTypes, imagesNames, soundsNames, soundsRanges, projectilesDamage, weaponAttackSpeed, projectilesSpeed);

	// read end of level info
	getEndOfLevelInfo(level, imagesNames);

	// read health info
	getHealthInfo(level, imagesNames);

	double timerLevelDuration;
	getTimerLevelInfo(level, timerLevelDuration, soundsNames, soundsRanges);

	/// initialize objects
	unsigned int numbersOfLevels = 0;
	std::vector<sf::Vector2u> wallsCoords;
	sf::Vector2u endOfLevelCoords;
	std::vector<sf::Vector2u> monstersCoords;
	std::vector<sf::Vector2u> weaponsCoords;
	getGeneralInfo(level, numbersOfLevels, wallsCoords, endOfLevelCoords, monstersCoords, weaponsCoords);

	m_gameObjects.push_back(new Background(0, 0, m_windowDimensions.w, m_windowDimensions.h, false, AUDIO_FOLDER + backgroundMusicFileName));
	resourcesTypes.push_back(OBJ_TYPE::BACKGROUND_TYPE);

	// initialize Zero 
	double movingObjWidth = getGameObjSize().x / 2;
	double movingObjHeight = getGameObjSize().y / 2;
	sf::Vector2u offsetFromTopLeft(getLevelBlockDimensions().x / 10, getLevelBlockDimensions().y / 10);
	m_gameObjects.push_back(
		new ZeroCharacter(
			offsetFromTopLeft.x, 
			offsetFromTopLeft.y, 
			movingObjWidth, 
			movingObjHeight, 
			zeroSpeed, 
			zeroHealth, 
			zeroAttcackingSpeed, 
			zeroFiringAccurracy));
	resourcesTypes.push_back(OBJ_TYPE::ZERO_TYPE);
	dynamic_cast<ZeroCharacter*>(m_gameObjects[1])->setWeapon(ZeroCurrentWeapon);

	// init walls
	for (auto wallCoord : wallsCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(wallCoord);
		m_gameObjects.push_back(new Wall(worldCoords.x, worldCoords.y, getGameObjSize().x, getGameObjSize().y, false));
	}
	resourcesTypes.push_back(OBJ_TYPE::WALL_TYPE);

	// init monsters
	if (monstersCoords.size() > 0)
	{
		for (auto monstCoord : monstersCoords)
		{
			sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(monstCoord);
			m_gameObjects.push_back(Monster::createMonster(monsterType,
				worldCoords.x + offsetFromTopLeft.x,
				worldCoords.y + offsetFromTopLeft.y,
				movingObjWidth,
				movingObjHeight,
				monsterDamage,
				monsterSpeed,
				monsterHealth,
				monsterAttackingSpeed,
				monsterImmuneFrom));
		}
		resourcesTypes.push_back(OBJ_TYPE::MONSTER_TYPE);
	}

	// init weapons
	for (auto weaponCoord : weaponsCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(weaponCoord);
		m_gameObjects.push_back(Weapon::createWeapon(allWeaponsTypes[allWeaponsTypes.size() - 1],
													worldCoords.x, 
													worldCoords.y, 
													movingObjWidth, 
													movingObjHeight, 
													false, 
													weaponAttackSpeed,
													allProjectilesTypes[allProjectilesTypes.size() - 1],
													projectilesDamage,
													projectilesSpeed));
	}
	for (auto& type : allWeaponsTypes)
	{
		resourcesTypes.push_back(type);
	}
	for (auto& type : allProjectilesTypes)
	{
		resourcesTypes.push_back(type);
	}
	if (ZeroCurrentWeapon)
		m_gameObjects.push_back(ZeroCurrentWeapon);

	// init end of level
	sf::Vector2u eolWorldCoords = calcWorldCoordsFromMapCoords(endOfLevelCoords);
	m_gameObjects.push_back(new EndOfLevel(
											eolWorldCoords.x, 
											eolWorldCoords.y, 
											getGameObjSize().x, 
											getGameObjSize().y, 
											false, 
											(level == numbersOfLevels)));
	resourcesTypes.push_back(OBJ_TYPE::END_OF_LEVEL_TYPE);

	// init health
	resourcesTypes.push_back(OBJ_TYPE::HEALTH_TYPE);
	resourcesTypes.push_back(OBJ_TYPE::HEALTH_BACKGROUND_TYPE);

	// init level timer
	m_gameObjects.push_back(new LevelHeader(
		offsetFromTopLeft.x, 
		0, 
		m_windowDimensions.w - offsetFromTopLeft.x, 
		getGameObjSize().y / 2, 
		timerLevelDuration));
}

void ResourcesManager::loadMenus(umapTypeString& imagesNames, umapTypeString& musicNames, umapTypeVecStrings& soundsNames, umapTypeVecInts& soundsRanges)
{
	mapStrStr fileCommandsToResources;
	FileReadWriteTools::extractFileCommandsAndResources(MENUS_INFO_FILE_PATH, fileCommandsToResources);
	//// regular menu
	imagesNames[OBJ_TYPE::MENU_BUTTON_TYPE] = fileCommandsToResources["buttonsTexture"];
	FileReadWriteTools::extractSoundsFileNames(MENUS_INFO_FILE_PATH, OBJ_TYPE::MENU_BUTTON_TYPE, soundsNames, soundsRanges, "numOfButtonsSounds");
	imagesNames[OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE] = fileCommandsToResources["highlighterTexture"];
	imagesNames[OBJ_TYPE::MENU_TYPE] = fileCommandsToResources["menuTexture"];
	musicNames[OBJ_TYPE::MENU_TYPE] = fileCommandsToResources["menuBackgroundMusic"];
	FileReadWriteTools::extractSoundsFileNames(MENUS_INFO_FILE_PATH, OBJ_TYPE::MENU_TYPE, soundsNames, soundsRanges, "numOfButtonsSounds");

	//// start screen
	imagesNames[OBJ_TYPE::START_SCREEN_TYPE] = fileCommandsToResources["startScreenTexture"];
	musicNames[OBJ_TYPE::START_SCREEN_TYPE] = fileCommandsToResources["startScreenBckgrMusic"];

	//// finished level screen
	imagesNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE] = fileCommandsToResources["finishedLevelTexture"];
	musicNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE] = fileCommandsToResources["finishedLevelBckgrMusic"];

	//// game over
	imagesNames[OBJ_TYPE::GAME_OVER_SCREEN_TYPE] = fileCommandsToResources["gameOverTexture"];
	musicNames[OBJ_TYPE::GAME_OVER_SCREEN_TYPE] = fileCommandsToResources["gameOverBckgrMusic"];

	//// final screen
	imagesNames[OBJ_TYPE::FINAL_SCREEN_TYPE] = fileCommandsToResources["finalScreenTexture"];
	musicNames[OBJ_TYPE::FINAL_SCREEN_TYPE] = fileCommandsToResources["finalScreenBckgrMusic"];
}

void ResourcesManager::initMenus(std::vector<OBJ_TYPE>& resCommands, umapTypeString& musicNames)
{
	resCommands.push_back(OBJ_TYPE::MENU_BUTTON_TYPE);
	resCommands.push_back(OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE);
	resCommands.push_back(OBJ_TYPE::MENU_TYPE);
	m_menus[RUN_MENU_STATE::MAIN_MENU_STATE] =
		new MainMenu(
			0, 0, 
			m_windowDimensions.w, m_windowDimensions.h, 
			false, 
			AUDIO_FOLDER + musicNames[OBJ_TYPE::MENU_TYPE]);
	m_menus[RUN_MENU_STATE::RESUME_MENU_STATE] =
		new ResumeGameMenu(
			0, 0,
			m_windowDimensions.w, m_windowDimensions.h,
			false,
			AUDIO_FOLDER + musicNames[OBJ_TYPE::MENU_TYPE]);
	m_menus[RUN_MENU_STATE::SAVE_GAME_MENU_STATE] =
		new SaveMenu(
			0, 0,
			m_windowDimensions.w, m_windowDimensions.h,
			false,
			AUDIO_FOLDER + musicNames[OBJ_TYPE::MENU_TYPE]);
	m_menus[RUN_MENU_STATE::LOAD_GAME_STATE] =
		new LoadMenu(
			0, 0,
			m_windowDimensions.w, m_windowDimensions.h,
			false,
			AUDIO_FOLDER + musicNames[OBJ_TYPE::MENU_TYPE]);
	resCommands.push_back(OBJ_TYPE::START_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::START_SCREEN_STATE] =
		new StartScreen(
			0, 0, 
			m_windowDimensions.w, m_windowDimensions.h, 
			false, START_SCREEN_SECONDS_TO_WAIT, 
			AUDIO_FOLDER + musicNames[OBJ_TYPE::START_SCREEN_TYPE]);
	resCommands.push_back(OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE] =
		new FinishedLevelScreen(
			0, 0, 
			m_windowDimensions.w, m_windowDimensions.h, 
			false, FINISHED_LEVEL_SCREEN_SECONDS_TO_WAIT, 
			AUDIO_FOLDER + musicNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE]);
	resCommands.push_back(OBJ_TYPE::GAME_OVER_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::GAME_OVER_SCREEN_STATE] = 
		new GameOverScreen(
			0, 0, 
			m_windowDimensions.w, m_windowDimensions.h, 
			false, GAME_OVER_SCREEN_SECONDS_TO_WAIT, 
			AUDIO_FOLDER + musicNames[OBJ_TYPE::GAME_OVER_SCREEN_TYPE]);
	resCommands.push_back(OBJ_TYPE::FINAL_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::FINAL_SCREEN_STATE] =
		new FinalScreen(0, 0, 
			m_windowDimensions.w, m_windowDimensions.h, 
			false, GAME_OVER_SCREEN_SECONDS_TO_WAIT,
			AUDIO_FOLDER + musicNames[OBJ_TYPE::FINAL_SCREEN_TYPE]);
}

void ResourcesManager::getZeroInfo(umapTypeString& imagesNames, 
								   umapTypeVecStrings& soundsNames,
								   umapTypeVecInts& soundsRanges,
								   double& zeroSpeed, 
								   double& zeroHealth, 
								   double& zeroAttcackingSpeed, 
								   double& zeroFiringAccurracy)
{
	mapStrStr fileCommandsAndResources;
	FileReadWriteTools::extractFileCommandsAndResources(ZERO_INFO_FILE_PATH, fileCommandsAndResources);
	imagesNames[OBJ_TYPE::ZERO_TYPE]	=						 fileCommandsAndResources["texture"];
	m_animations[OBJ_TYPE::ZERO_TYPE]	= getAnimationFromString(fileCommandsAndResources["animation"]);
	zeroSpeed							=			   std::stod(fileCommandsAndResources["speed"]);
	zeroHealth							=			   std::stod(fileCommandsAndResources["health"]);
	zeroAttcackingSpeed					=			   std::stod(fileCommandsAndResources["attackingSpeed"]);
	zeroFiringAccurracy					=			   std::stod(fileCommandsAndResources["firingAccuracy"]);
	FileReadWriteTools::extractSoundsFileNames(ZERO_INFO_FILE_PATH, OBJ_TYPE::ZERO_TYPE, soundsNames, soundsRanges);
}

void ResourcesManager::getBackgroundInfo(const unsigned int & level, umapTypeString& imagesNames, std::string& backgroundMusicFileName)
{
	mapStrStr fileCommandsAndResources;
	FileReadWriteTools::extractFileCommandsAndResources(BACKGROUND_INFO_FILE_PATH, fileCommandsAndResources, level);
	imagesNames[OBJ_TYPE::BACKGROUND_TYPE]	= fileCommandsAndResources["texture"];
	backgroundMusicFileName					= fileCommandsAndResources["backgroundMusic"];
}

void ResourcesManager::getWallsInfo(const unsigned int & level, umapTypeString& imagesNames)
{
	mapStrStr fileCommandsAndResources;
	FileReadWriteTools::extractFileCommandsAndResources(WALLS_INFO_FILE_PATH, fileCommandsAndResources, level);
	imagesNames[OBJ_TYPE::WALL_TYPE] = fileCommandsAndResources["texture"];
}

void ResourcesManager::getMonstersInfo(const unsigned int & level, 
									   MONSTERS_TYPE & monsterType, 
									   umapTypeString& imagesNames, 
									   umapTypeVecStrings& soundsNames,
									   umapTypeVecInts& soundsRanges,
									   double& damage, 
									   double& speed, 
									   double& health,
									   double& attackingSpeed,
									   std::vector<OBJ_TYPE>& monsterImmuneFrom)
{
	mapStrStr fileCommandsToResources;
	FileReadWriteTools::extractFileCommandsAndResources(MONSTERS_INFO_FILE_PATH, fileCommandsToResources, level);
	monsterType								= static_cast<MONSTERS_TYPE>(std::stoi(fileCommandsToResources["monsterType"]));
	imagesNames[OBJ_TYPE::MONSTER_TYPE]		=									   fileCommandsToResources["texture"];
	m_animations[OBJ_TYPE::MONSTER_TYPE]	=			    getAnimationFromString(fileCommandsToResources["animationFrames"]);
	damage									=							 std::stod(fileCommandsToResources["damage"]);
	speed									=							 std::stod(fileCommandsToResources["speed"]);
	health									=							 std::stod(fileCommandsToResources["health"]);
	attackingSpeed							=							 std::stod(fileCommandsToResources["attackingSpeed"]);
	int numProjTypes						=							 std::stod(fileCommandsToResources["numImmuneFromProjectiles"]);
	if (numProjTypes > 0)
	{
		std::string immuneFromProjectilesListAsStr = fileCommandsToResources["immuneFromProjectilesList"];
		std::stringstream ss;
		ss << immuneFromProjectilesListAsStr;
		std::string projectileTypeAsStr;
		for(int i = 0; i < numProjTypes; i++)
		{
			ss >> projectileTypeAsStr;
			monsterImmuneFrom.push_back(static_cast<OBJ_TYPE>(std::stoi(projectileTypeAsStr)));
		}
	}
	FileReadWriteTools::extractSoundsFileNames(MONSTERS_INFO_FILE_PATH, OBJ_TYPE::MONSTER_TYPE, soundsNames, soundsRanges, "numberOfSoundsLv" + std::to_string(level));
}

void ResourcesManager::getWeaponsInfo(const unsigned int & level,
									   std::vector<OBJ_TYPE>& weaponsTypes,
									   std::vector<OBJ_TYPE>& projectilesTypes,
									   umapTypeString& imagesNames,
									   umapTypeVecStrings& soundsNames,
									   umapTypeVecInts& soundsRanges,
									   double& projectilesDamage,
									   double& firingRate,
									   double& projectilesSpeed)
{
	mapStrStr filesCommandsToResources;
	FileReadWriteTools::extractFileCommandsAndResources(WEAPONS_INFO_FILE_PATH, filesCommandsToResources, level);
	OBJ_TYPE weaponType = 
		static_cast<OBJ_TYPE>(
			std::stoi(filesCommandsToResources["weaponType"]) + 
			static_cast<int>(OBJ_TYPE::WEAPONS_TYPES_START));
	bool hasWeapon = (weaponType != OBJ_TYPE::WEAPONS_TYPES_START);
	if (hasWeapon)
	{
		weaponsTypes.push_back(weaponType);
		imagesNames[weaponType]		=						 filesCommandsToResources["weaponTexture"];
		m_animations[weaponType]	= getAnimationFromString(filesCommandsToResources["weaponAnimFrames"]);
		firingRate					=			   std::stod(filesCommandsToResources["firingRate"]);
		OBJ_TYPE projectilesType =
			static_cast<OBJ_TYPE>(
				std::stoi(filesCommandsToResources["projectileType"]) +
				static_cast<int>(OBJ_TYPE::PROJECTILES_TYPES_START));
		projectilesTypes.push_back(projectilesType);
		imagesNames[projectilesType]	=			filesCommandsToResources["projectileTexture"];
		projectilesDamage				= std::stod(filesCommandsToResources["projectilesDamage"]);
		projectilesSpeed				= std::stod(filesCommandsToResources["projectilesSpeed"]);
		FileReadWriteTools::extractSoundsFileNames(WEAPONS_INFO_FILE_PATH, weaponType, soundsNames, soundsRanges, "numberOfSoundsLv" + std::to_string(level));
	}
}

void ResourcesManager::getEndOfLevelInfo(const unsigned int & level, umapTypeString& imagesNames)
{
	mapStrStr filesCommandsToResources;
	FileReadWriteTools::extractFileCommandsAndResources(END_OF_LEVEL_INFO_FILE_PATH, filesCommandsToResources);
	imagesNames[OBJ_TYPE::END_OF_LEVEL_TYPE] = filesCommandsToResources["texture"];
}

void ResourcesManager::getHealthInfo(const unsigned int & level, umapTypeString& imagesNames)
{
	mapStrStr filesCommandsToResources;
	FileReadWriteTools::extractFileCommandsAndResources(HEALTH_INFO_FILE_PATH, filesCommandsToResources);
	imagesNames[OBJ_TYPE::HEALTH_TYPE]				= filesCommandsToResources["healthTexture"];
	imagesNames[OBJ_TYPE::HEALTH_BACKGROUND_TYPE]	= filesCommandsToResources["healthBackgroundTexture"];
}

void ResourcesManager::getTimerLevelInfo(const unsigned int& level, double& timeDuration, umapTypeVecStrings& soundNames, umapTypeVecInts& soundRanges)
{
	mapStrStr filesCommandsToResources;
	FileReadWriteTools::extractFileCommandsAndResources(TIMER_LEVEL_FILE_PATH, filesCommandsToResources, level);
	timeDuration = std::stod(filesCommandsToResources["timeDuration"]);
	FileReadWriteTools::extractSoundsFileNames(TIMER_LEVEL_FILE_PATH, OBJ_TYPE::LEVEL_HEADER_TYPE, soundNames, soundRanges);
}

void ResourcesManager::getGeneralInfo(const unsigned int & level, 
									  unsigned int & numbersOfLevels, 
									  std::vector<sf::Vector2u>& wallsCoords, 
									  sf::Vector2u & endOfLevelCoords, 
									  std::vector<sf::Vector2u>& monstersCoords,
									  std::vector<sf::Vector2u>& weaponsCoords)
{
	mapStrStr fileCommandsToResources;
	// 1. get the whole info because numbersOfLevels is out of any level
	FileReadWriteTools::extractFileCommandsAndResources(GENERAL_INFO_FILE_PATH, fileCommandsToResources); 
	numbersOfLevels = std::stoi(fileCommandsToResources["numOfLevels"]);
	// 2. get level specific info
	fileCommandsToResources.clear();
	FileReadWriteTools::extractFileCommandsAndResources(GENERAL_INFO_FILE_PATH, fileCommandsToResources, level);
	
	// clear the coordinates form the previous level if there were any
	wallsCoords.clear();
	monstersCoords.clear();
	weaponsCoords.clear();

	// get stage info
	m_objectsInLevel.x = std::stoi(fileCommandsToResources["columnsNum"]);
	m_objectsInLevel.y = std::stoi(fileCommandsToResources["rowsNum"]);
	std::string currentRow = "";
	for (unsigned int i = 0; i < m_objectsInLevel.y; ++i) // rows, i.e. y
	{
		currentRow = fileCommandsToResources["row" + std::to_string(i)];
		std::stringstream ss;
		ss << currentRow;
		for (unsigned int j = 0; j < m_objectsInLevel.x; ++j) // cols, i.e. x
		{
			std::string typeOfObjStr;
			ss >> typeOfObjStr;
			objTypeOnLevelMap typeOfObjInt = static_cast<objTypeOnLevelMap>(std::stoi(typeOfObjStr));
			switch (typeOfObjInt)
			{
				case objTypeOnLevelMap::MAP_WALL_OBJ:
					wallsCoords.push_back(sf::Vector2u(j, i));
					break;
				case objTypeOnLevelMap::MAP_MONSTER_OBJ:
					monstersCoords.push_back(sf::Vector2u(j, i));
					break;
				case objTypeOnLevelMap::MAP_WEAPON_OBJ:
					weaponsCoords.push_back(sf::Vector2u(j, i));
					break;
				case objTypeOnLevelMap::MAP_END_OF_LEVEL_OBJ:
					endOfLevelCoords = sf::Vector2u(j, i);
					break;
				default:
					break;
			}
		}
	}

	setSpeedFactor();
}

void ResourcesManager::setSpeedFactor()
{
	m_speedFactor.x = getLevelBlockDimensions().x / m_speedFactroDivider;
	m_speedFactor.y = getLevelBlockDimensions().y / m_speedFactroDivider; 
}


const sf::Vector2f ResourcesManager::getLevelBlockDimensions()
{
	return sf::Vector2f(m_windowDimensions.w / m_objectsInLevel.x, m_windowDimensions.h / m_objectsInLevel.y);
}

std::pair<std::string, sf::Texture> ResourcesManager::getTexture(OBJ_TYPE objType)
{
	return m_textures[objType];
}

bool ResourcesManager::getAnimation(OBJ_TYPE command, Animation& animation)
{
	bool rc = false;
	for(auto it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		if (it->first == command)
		{
			animation = m_animations[command];
			rc = true;
			break;
		}
	}

	return rc;
}

SoundBuffersHolder& ResourcesManager::getSoundBuffers(OBJ_TYPE type, bool& noBuffers)
{
	if (m_soundBuffers.find(type) == m_soundBuffers.end())
	{
		noBuffers = true;
		return SoundBuffersHolder(); // dummy return
	}

	noBuffers = false;
	return m_soundBuffers[type];
}