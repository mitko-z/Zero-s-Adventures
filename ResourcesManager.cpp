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
	imagesNames[OBJ_TYPE::MENU_BUTTON_TYPE] = "MenuButtonEmptyWide.png";
	resCommands.push_back(OBJ_TYPE::MENU_BUTTON_TYPE);
	imagesNames[OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE] = "frame-transparent.png";
	resCommands.push_back(OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE);
	imagesNames[OBJ_TYPE::MENU_TYPE] = "MenuBackgroundRectangle.png";
	m_menus[RUN_MENU_STATE::MAIN_MENU_STATE] =
		new MainMenu(0, 0, m_windowDimensions.w, m_windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::MENU_TYPE);

	imagesNames[OBJ_TYPE::START_SCREEN_TYPE] = "StartScreen.png";
	m_menus[RUN_MENU_STATE::START_SCREEN_STATE] =
		new StartScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::START_SCREEN_TYPE);

	imagesNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE] = "finished level screen image.png";
	m_menus[RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE] =
		new FinishedLevelScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE);

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
	m_gameObjects.clear();
	imagesNames.clear();
	resCommands.clear();
	m_animations.clear();

	std::string loadPath("Data/info.dat");
	std::ifstream levelsReader(loadPath);

	if (!levelsReader)
	{
		std::string throwMessage = "Cannot load levels info file: " + loadPath;
		throw throwMessage;
	}

	std::string lineRead;
	std::getline(levelsReader, lineRead);	// read "; name of Zero's texture.."
	std::getline(levelsReader, lineRead);	// read the name of the texture
	imagesNames[OBJ_TYPE::ZERO_TYPE] = lineRead + ".png";
	std::getline(levelsReader, lineRead);	// read "; Zero texture frames x, y"
	std::getline(levelsReader, lineRead);	// read the frames upon x and y
	m_animations[OBJ_TYPE::ZERO_TYPE] = getAnimationFromString(lineRead);
	std::getline(levelsReader, lineRead);	// read "; number of levels"
	std::getline(levelsReader, lineRead);	// read the number of levels
	unsigned int numbersOfLevels = std::stoi(lineRead);

	unsigned int currentLevel = 0;
	std::vector<sf::Vector2u> wallsCoords;
	sf::Vector2u endOfLevelCoords;
	std::vector<sf::Vector2u> monstersCoords;
	MONSTER_TYPES monsterType = MONSTER_TYPES::NO_MONSTER_TYPE;
	do
	{
		// clear the coordinates form the previous level 
		wallsCoords.clear();
		monstersCoords.clear();

		// read from file
		
		// read level info
		std::getline(levelsReader, lineRead);	// read "; level no."
		std::getline(levelsReader, lineRead);	// read 1
		currentLevel = std::stoi(lineRead);
		// read background info 
		std::getline(levelsReader, lineRead);	// read "; name for the texture of the background"
		std::getline(levelsReader, lineRead);	// read the name for the texture of the background
		imagesNames[OBJ_TYPE::BACKGROUND_TYPE] = lineRead + ".png";

		// read walls info
		std::getline(levelsReader, lineRead);	// read  ; name for the texture of walls (without the extension of the file)
		std::getline(levelsReader, lineRead);	// read  the name for the texture of walls
		imagesNames[OBJ_TYPE::WALL_TYPE] = lineRead + ".png";

		// read monsters info
		std::getline(levelsReader, lineRead);	// read ; index number of the monster type for this level
		std::getline(levelsReader, lineRead);	// read the index number of the monster type for this level
		monsterType = static_cast<MONSTER_TYPES>(std::stoi(lineRead));
		std::getline(levelsReader, lineRead);	// read  ; name for the texture of monsters (without the extension of the file)
		std::getline(levelsReader, lineRead);	// read  the name for the texture of monsters
		imagesNames[OBJ_TYPE::MONSTER_TYPE] = lineRead + ".png";
		std::getline(levelsReader, lineRead);	// read "; Zero texture frames x, y"
		std::getline(levelsReader, lineRead);	// read the frames upon x and y
		m_animations[OBJ_TYPE::MONSTER_TYPE] = getAnimationFromString(lineRead);

		// read weapon info
		std::getline(levelsReader, lineRead);	// read ; index number for the type of weapon for this level
		std::getline(levelsReader, lineRead);	// read the index number for the type of weapon for this level
		// imagesNames[OBJ_TYPE::/**/] = lineRead + ".png"; // TODO set later

		// read end of level info
		imagesNames[OBJ_TYPE::END_OF_LEVEL_TYPE] = "mushroomHouse.png";

		// read stage info
		std::getline(levelsReader, lineRead);	// read ; number of columns for the level
		std::getline(levelsReader, lineRead);	// read the number of columns for the level
		m_objectsInLevel.x = std::stoi(lineRead);
		std::getline(levelsReader, lineRead);	// read ; number of rows for the level
		std::getline(levelsReader, lineRead);	// read the number of rows for the level
		m_objectsInLevel.y = std::stoi(lineRead);
		std::getline(levelsReader, lineRead);	// read ; table of the level; legend: 0 - empty space, 1 - wall, 2 - monster; 9 - end of level

		for (unsigned int i = 0; i < m_objectsInLevel.y; ++i) // rows, i.e. y
		{
			std::getline(levelsReader, lineRead);
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
	}
	while (currentLevel != level);

	levelsReader.close();

	m_gameObjects.push_back(new Background(0, 0, m_windowDimensions.w, m_windowDimensions.h, false));
	resCommands.push_back(OBJ_TYPE::BACKGROUND_TYPE);

	// initialize Zero AFTER background in order not to draw background upon Zero
	double zeroWidth = getGameObjSize().x / 2;
	double zeroHeight = getGameObjSize().y / 2;
	m_gameObjects.push_back(new ZeroCharacter(0, 0, zeroWidth, zeroHeight));
	resCommands.push_back(OBJ_TYPE::ZERO_TYPE);

	// init walls
	for (auto wallCoord : wallsCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(wallCoord);
		m_gameObjects.push_back(new Wall(worldCoords.x, worldCoords.y, getGameObjSize().x, getGameObjSize().y, false));
	}
	resCommands.push_back(OBJ_TYPE::WALL_TYPE);

	// init monster
	for (auto monstCoord : monstersCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(monstCoord);
		m_gameObjects.push_back(Monster::createMonster(monsterType, worldCoords.x, worldCoords.y, getGameObjSize().x, getGameObjSize().y));
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
											(currentLevel == numbersOfLevels)));
	resCommands.push_back(OBJ_TYPE::END_OF_LEVEL_TYPE);
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