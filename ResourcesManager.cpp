#include "ResourcesManager.h"

#include <sstream>

#include "ZeroCharacter.h"
#include "Background.h"
#include "MainMenu.h"
#include "StartScreen.h"
#include "Wall.h"
#include "EndOfLevel.h"
#include "FinishedLevelScreen.h"
#include "Monster.h"
#include "Health.h"

#define GENERAL_INFO_FILE_PATH		"Data/general_info.dat"
#define WALLS_INFO_FILE_PATH		"Data/walls_info.dat"
#define BACKGROUND_INFO_FILE_PATH	"Data/background_info.dat"
#define ZERO_INFO_FILE_PATH			"Data/zero_info.dat"
#define MONSTERS_INFO_FILE_PATH		"Data/monsters_info.dat"
#define WEAPONS_INFO_FILE_PATH		"Data/weapons_info.dat"
#define END_OF_LEVEL_INFO_FILE_PATH "Data/end_of_level_info.dat"
#define HEALTH_INFO_FILE_PATH		"Data/health_info.dat"
#define MENUS_INFO_FILE_PATH		"Data/menus_info.dat"

ResourcesManager* ResourcesManager::m_instance = nullptr;

ResourcesManager* ResourcesManager::getInstance()
{
	if (!m_instance)
	{
		m_instance = new ResourcesManager;
	}
	return m_instance;
}

ResourcesManager::~ResourcesManager()
{
	delete m_instance;
}

void ResourcesManager::setWindowDimensions(float w, float h)
{
	m_windowDimensions.w = w;
	m_windowDimensions.h = h;
}

void ResourcesManager::loadResources(unsigned int level)
{
	// TODO figure out how to read any level defined by the parameter
	UMAP<OBJ_TYPE, std::string> imagesNames;
	std::vector<OBJ_TYPE> resCommands;

	loadLevel(level, imagesNames, resCommands);

	// Menu load
	if (level == 1)
	{
		loadMenus(imagesNames);
		initMenus(resCommands);
	}

	for (auto command : resCommands)
	{
		sf::Texture texture;
		std::string loadPath("Data/Images/");
		loadPath += imagesNames[command];
		if (!texture.loadFromFile(loadPath))
		{
			std::string throwMessage = "Cannot load image " + loadPath;
			throw throwMessage;
		}
		m_textures[command] = texture;
	} // end for each command
}

Animation ResourcesManager::getAnimationFromString(std::string strData)
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
								 UMAP<OBJ_TYPE, std::string>& imagesNames,
								 std::vector<OBJ_TYPE>& resCommands)
{
	// clear all objects so they can be loaded freshly
	m_gameObjects.clear();
	imagesNames.clear();
	resCommands.clear();
	m_animations.clear();

	double zeroSpeed, zeroHealth;
	getZeroInfo(imagesNames, zeroSpeed, zeroHealth);
	getBackgroundInfo(level, imagesNames);
	getWallsInfo(level, imagesNames);
	MONSTER_TYPES monsterType = MONSTER_TYPES::NO_MONSTER_TYPE;
	double monsterDamage, monsterSpeed, monsterHealth, monsterAttackingSpeed;
	getMonstersInfo(level, monsterType, imagesNames, monsterDamage, monsterSpeed, monsterHealth, monsterAttackingSpeed);
	getWeaponsInfo(level, imagesNames);
	getEndOfLevelInfo(level, imagesNames);
	getHealthInfo(level, imagesNames);

	unsigned int numbersOfLevels = 0;
	std::vector<sf::Vector2u> wallsCoords;
	sf::Vector2u endOfLevelCoords;
	std::vector<sf::Vector2u> monstersCoords;
	getGeneralInfo(level, numbersOfLevels, wallsCoords, endOfLevelCoords, monstersCoords);

	m_gameObjects.push_back(new Background(0, 0, m_windowDimensions.w, m_windowDimensions.h, false));
	resCommands.push_back(OBJ_TYPE::BACKGROUND_TYPE);

	// initialize Zero AFTER background in order not to draw background upon Zero
	double movingObjWidth = getGameObjSize().x / 2;
	double movingObjHeight = getGameObjSize().y / 2;
	m_gameObjects.push_back(new ZeroCharacter(0, 0, movingObjWidth, movingObjHeight, zeroSpeed, zeroHealth));
	resCommands.push_back(OBJ_TYPE::ZERO_TYPE);

	// init walls
	for (auto wallCoord : wallsCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(wallCoord);
		m_gameObjects.push_back(new Wall(worldCoords.x, worldCoords.y, getGameObjSize().x, getGameObjSize().y, false));
	}
	resCommands.push_back(OBJ_TYPE::WALL_TYPE);

	// init monsters
	for (auto monstCoord : monstersCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(monstCoord);
		m_gameObjects.push_back(Monster::createMonster(monsterType, 
													   worldCoords.x, 
													   worldCoords.y, 
													   movingObjWidth, 
													   movingObjHeight, 
													   monsterDamage, 
													   monsterSpeed,
													   monsterHealth,
													   monsterAttackingSpeed));
	}
	if (monstersCoords.size() > 0)
	{
		resCommands.push_back(OBJ_TYPE::MONSTER_TYPE);
	}

	// init end of level
	sf::Vector2u eolWorldCoords = calcWorldCoordsFromMapCoords(endOfLevelCoords);
	m_gameObjects.push_back(new EndOfLevel(
											eolWorldCoords.x, 
											eolWorldCoords.y, 
											getGameObjSize().x, 
											getGameObjSize().y, 
											false, 
											(level == numbersOfLevels)));
	resCommands.push_back(OBJ_TYPE::END_OF_LEVEL_TYPE);

	// init health
	resCommands.push_back(OBJ_TYPE::HEALTH_TYPE);
	resCommands.push_back(OBJ_TYPE::HEALTH_BACKGROUND_TYPE);
}

void ResourcesManager::loadMenus(UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(MENUS_INFO_FILE_PATH);
	std::string lineRead;
	std::getline(infoReader, lineRead);	// read "; name of the buttons texture (without the extension of the file)"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::MENU_BUTTON_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; name of the buttons highlighter texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; name of the menus background texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::MENU_TYPE] = lineRead + ".png";

	std::getline(infoReader, lineRead);	// read "; name of the start screen texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::START_SCREEN_TYPE] = lineRead + ".png";

	std::getline(infoReader, lineRead);	// read "; name of the start screen texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE] = lineRead + ".png";
}

void ResourcesManager::initMenus(std::vector<OBJ_TYPE>& resCommands)
{
	resCommands.push_back(OBJ_TYPE::MENU_BUTTON_TYPE);
	resCommands.push_back(OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE);
	resCommands.push_back(OBJ_TYPE::MENU_TYPE);
	m_menus[RUN_MENU_STATE::MAIN_MENU_STATE] =
		new MainMenu(0, 0, m_windowDimensions.w, m_windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::START_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::START_SCREEN_STATE] =
		new StartScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE] =
		new FinishedLevelScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false);
}

std::ifstream ResourcesManager::getReader(std::string filePath)
{
	std::string loadPath(filePath);
	std::ifstream reader(loadPath);
	if (!reader)
	{
		std::string throwMessage = "Cannot load levels info file: " + loadPath;
		throw throwMessage;
	}
	return reader;
}

void ResourcesManager::getZeroInfo(UMAP<OBJ_TYPE, std::string>& imagesNames, double& zeroSpeed, double& zeroHealth)
{
	std::ifstream infoReader = getReader(ZERO_INFO_FILE_PATH);
	std::string lineRead;
	std::getline(infoReader, lineRead);	// read "; name of Zero's texture.."
	std::getline(infoReader, lineRead);	// read the name of the texture
	imagesNames[OBJ_TYPE::ZERO_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; Zero texture frames x, y"
	std::getline(infoReader, lineRead);	// read the frames upon x and y
	m_animations[OBJ_TYPE::ZERO_TYPE] = getAnimationFromString(lineRead);
	std::getline(infoReader, lineRead);	// read ; Zero speed
	std::getline(infoReader, lineRead);	// read Zero's speed
	zeroSpeed = std::stod(lineRead);
	std::getline(infoReader, lineRead);	// read ; Zero health
	std::getline(infoReader, lineRead);	// read Zero's health
	zeroHealth = std::stod(lineRead);
	infoReader.close();
}

void ResourcesManager::getBackgroundInfo(const unsigned int & level, UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(BACKGROUND_INFO_FILE_PATH);
	std::string lineRead;
	unsigned int currentLevel = 0;
	do
	{
		// read level info
		std::getline(infoReader, lineRead);	// read "; level no."
		std::getline(infoReader, lineRead);	// read the level no
		currentLevel = std::stoi(lineRead);

		// read background info 
		std::getline(infoReader, lineRead);	// read "; name for the texture of the background"
		std::getline(infoReader, lineRead);	// read the name for the texture of the background
		imagesNames[OBJ_TYPE::BACKGROUND_TYPE] = lineRead + ".png";
	} while (currentLevel != level);
	infoReader.close();
}

void ResourcesManager::getWallsInfo(const unsigned int & level, UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(WALLS_INFO_FILE_PATH);
	std::string lineRead;
	unsigned int currentLevel = 0;
	do
	{
		// read level info
		std::getline(infoReader, lineRead);	// read "; level no."
		std::getline(infoReader, lineRead);	// read the level no
		currentLevel = std::stoi(lineRead);

		// read walls info
		std::getline(infoReader, lineRead);	// read  ; name for the texture of walls (without the extension of the file)
		std::getline(infoReader, lineRead);	// read  the name for the texture of walls
		imagesNames[OBJ_TYPE::WALL_TYPE] = lineRead + ".png";
	} 
	while (currentLevel != level);
	infoReader.close();
}

void ResourcesManager::getMonstersInfo(const unsigned int & level, 
									   MONSTER_TYPES & monsterType, 
									   UMAP<OBJ_TYPE, std::string>& imagesNames, 
									   double&damage, 
									   double& speed, 
									   double& health,
									   double& attackingSpeed)
{
	std::ifstream infoReader = getReader(MONSTERS_INFO_FILE_PATH);
	std::string lineRead;
	unsigned int currentLevel = 0;
	do
	{
		// read level info
		std::getline(infoReader, lineRead);	// read "; level no."
		std::getline(infoReader, lineRead);	// read the level no
		currentLevel = std::stoi(lineRead);

		// read monsters info
		std::getline(infoReader, lineRead);	// read ; index number of the monster type for this level
		std::getline(infoReader, lineRead);	// read the index number of the monster type for this level
		monsterType = static_cast<MONSTER_TYPES>(std::stoi(lineRead));
		std::getline(infoReader, lineRead);	// read  ; name for the texture of monsters (without the extension of the file)
		std::getline(infoReader, lineRead);	// read  the name for the texture of monsters
		imagesNames[OBJ_TYPE::MONSTER_TYPE] = lineRead + ".png";
		std::getline(infoReader, lineRead);	// read "Monster texture frames x, y"
		std::getline(infoReader, lineRead);	// read the frames upon y and x
		m_animations[OBJ_TYPE::MONSTER_TYPE] = getAnimationFromString(lineRead);
		std::getline(infoReader, lineRead);	// read ; Monster damage
		std::getline(infoReader, lineRead);	// read the monster damage
		damage = std::stod(lineRead);
		std::getline(infoReader, lineRead);	// read ; Monster speed
		std::getline(infoReader, lineRead);	// read the monster speed
		speed = std::stod(lineRead);
		std::getline(infoReader, lineRead);	// read ; Monster health
		std::getline(infoReader, lineRead);	// read the monster health
		health = std::stod(lineRead);
		std::getline(infoReader, lineRead);	// read ; Monster attacking speed
		std::getline(infoReader, lineRead);	// read the monster attacking speed
		attackingSpeed = std::stod(lineRead);
	} while (currentLevel != level);
	infoReader.close();
}

void ResourcesManager::getWeaponsInfo(const unsigned int & level, UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(WEAPONS_INFO_FILE_PATH);
	std::string lineRead;
	unsigned int currentLevel = 0;
	do
	{
		// read level info
		std::getline(infoReader, lineRead);	// read "; level no."
		std::getline(infoReader, lineRead);	// read the level no
		currentLevel = std::stoi(lineRead);

		// read weapon info
		std::getline(infoReader, lineRead);	// read ; index number for the type of weapon for this level
		std::getline(infoReader, lineRead);	// read the index number for the type of weapon for this level
		// imagesNames[OBJ_TYPE::/**/] = lineRead + ".png"; // TODO set later
	} while (currentLevel != level);
	infoReader.close();
}

void ResourcesManager::getEndOfLevelInfo(const unsigned int & level, UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(END_OF_LEVEL_INFO_FILE_PATH);
	std::string lineRead;

	std::getline(infoReader, lineRead);	// read "; name of texture for the end of level object withot the .png extension"
	std::getline(infoReader, lineRead);	// read the name for the texture
	imagesNames[OBJ_TYPE::END_OF_LEVEL_TYPE] = lineRead + ".png";
	infoReader.close();
}

void ResourcesManager::getHealthInfo(const unsigned int & level, UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(HEALTH_INFO_FILE_PATH);
	std::string lineRead;

	std::getline(infoReader, lineRead);	// read "; name of texture for the health object withot the .png extension"
	std::getline(infoReader, lineRead);	// read the name for the texture
	imagesNames[OBJ_TYPE::HEALTH_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; name of texture for the background health object withot the .png extension"
	std::getline(infoReader, lineRead);	// read the name for the texture
	imagesNames[OBJ_TYPE::HEALTH_BACKGROUND_TYPE] = lineRead + ".png";
	infoReader.close();
}

void ResourcesManager::getGeneralInfo(const unsigned int & level, 
									  unsigned int & numbersOfLevels, 
									  std::vector<sf::Vector2u>& wallsCoords, 
									  sf::Vector2u & endOfLevelCoords, 
									  std::vector<sf::Vector2u>& monstersCoords)
{
	std::ifstream generalInfoReader = getReader(GENERAL_INFO_FILE_PATH);
	std::string lineRead;
	std::getline(generalInfoReader, lineRead);	// read "; number of levels"
	std::getline(generalInfoReader, lineRead);	// read the number of levels
	numbersOfLevels = std::stoi(lineRead);

	unsigned int currentLevel = 0;
	do
	{
		// clear the coordinates form the previous level if there were any
		wallsCoords.clear();
		monstersCoords.clear();

		// read from file

		// read level info
		std::getline(generalInfoReader, lineRead);	// read "; level no."
		std::getline(generalInfoReader, lineRead);	// read the level no
		currentLevel = std::stoi(lineRead);

		// read stage info
		std::getline(generalInfoReader, lineRead);	// read ; number of columns for the level
		std::getline(generalInfoReader, lineRead);	// read the number of columns for the level
		m_objectsInLevel.x = std::stoi(lineRead);
		std::getline(generalInfoReader, lineRead);	// read ; number of rows for the level
		std::getline(generalInfoReader, lineRead);	// read the number of rows for the level
		m_objectsInLevel.y = std::stoi(lineRead);
		std::getline(generalInfoReader, lineRead);	// read ; table of the level; legend: 0 - empty space, 1 - wall, 2 - monster; 9 - end of level

		for (unsigned int i = 0; i < m_objectsInLevel.y; ++i) // rows, i.e. y
		{
			std::getline(generalInfoReader, lineRead);
			std::stringstream ss;
			ss << lineRead;
			for (unsigned int j = 0; j < m_objectsInLevel.x; ++j) // cols, i.e. x
			{
				std::string typeOfObjStr;
				ss >> typeOfObjStr;
				objTypeOnLevelMap typeOfObjInt = static_cast<objTypeOnLevelMap>(std::stoi(typeOfObjStr));
				switch (typeOfObjInt)
				{
				case objTypeOnLevelMap::WALL:
					wallsCoords.push_back(sf::Vector2u(j, i));
					break;
				case objTypeOnLevelMap::MONSTER:
					monstersCoords.push_back(sf::Vector2u(j, i));
					break;
				case objTypeOnLevelMap::END_OF_LEVEL:
					endOfLevelCoords = sf::Vector2u(j, i);
					break;
				default:
					break;
				}
			}
		}
	} while (currentLevel != level);

	generalInfoReader.close();
}

sf::Texture ResourcesManager::getTexture(OBJ_TYPE command)
{
	return m_textures[command];
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