#include "ResourcesManager.h"

#include <sstream>

#include "ZeroCharacter.h"
#include "Background.h"
#include "MainMenu.h"
#include "StartScreen.h"
#include "Wall.h"
#include "EndOfLevel.h"

ResourcesManager* ResourcesManager::instance = nullptr;

ResourcesManager* ResourcesManager::getInstance()
{
	if (!instance)
	{
		instance = new ResourcesManager;
	}
	return instance;
}

ResourcesManager::~ResourcesManager()
{
	delete instance;
}

void ResourcesManager::setWindowDimensions(float w, float h)
{
	windowDimensions.w = w;
	windowDimensions.h = h;
}

void ResourcesManager::loadResources(unsigned int level)
{
	// TODO figure out how to read any level defined by the parameter
	std::string loadPath("Data/info.dat");
	std::ifstream levelsReader(loadPath);

	if (!levelsReader)
	{
		std::string throwMessage = "Cannot load image " + loadPath;
		throw throwMessage;
	}

	std::string lineRead;
	UMAP<OBJ_TYPE, std::string> imagesNames;
	std::getline(levelsReader, lineRead);	// read "; name of Zero's texture.."
	std::getline(levelsReader, lineRead);	// read the name of the texture
	imagesNames[OBJ_TYPE::ZERO_TYPE] = lineRead + ".png";
	std::getline(levelsReader, lineRead);	// read "; Zero texture frames x, y"
	std::getline(levelsReader, lineRead);	// read the frames upon x and y
	animations[OBJ_TYPE::ZERO_TYPE] = getAnimationFromString(lineRead);
	std::getline(levelsReader, lineRead);	// read "; number of levels"
	std::getline(levelsReader, lineRead);	// read the number of levels
	std::getline(levelsReader, lineRead);	// read "; level no."
	std::getline(levelsReader, lineRead);	// read 1
	std::getline(levelsReader, lineRead);	// read "; name for the texture of the background"
	std::getline(levelsReader, lineRead);	// read the name for the texture of the background
	imagesNames[OBJ_TYPE::BACKGROUND_TYPE] = lineRead + ".png";
	std::getline(levelsReader, lineRead);	// read  ; name for the texture of walls (without the extension of the file)
	std::getline(levelsReader, lineRead);	// read  the name for the texture of walls
	imagesNames[OBJ_TYPE::WALL_TYPE] = lineRead + ".png";
	std::getline(levelsReader, lineRead);	// read ; index number of the creature type for this level
	std::getline(levelsReader, lineRead);	// read the index number of the creature type for this level
	// imagesNames[OBJ_TYPE::/**/] = lineRead + ".png"; // TODO set later
	std::getline(levelsReader, lineRead);	// read ; index number for the type of weapon for this level
	std::getline(levelsReader, lineRead);	// read the index number for the type of weapon for this level
	// imagesNames[OBJ_TYPE::/**/] = lineRead + ".png"; // TODO set later
	std::getline(levelsReader, lineRead);	// read ; number of columns for the level
	std::getline(levelsReader, lineRead);	// read the number of columns for the level
	objectsInLevel.x = std::stoi(lineRead);
	std::getline(levelsReader, lineRead);	// read ; number of rows for the level
	std::getline(levelsReader, lineRead);	// read the number of rows for the level
	objectsInLevel.y = std::stoi(lineRead);
	std::getline(levelsReader, lineRead);	// read ; table of the level; legend: 0 - empty space, 1 - wall, 2 - creature; 9 - end of level

	imagesNames[OBJ_TYPE::END_OF_LEVEL_TYPE] = "mushroomHouse.png";

	std::vector<sf::Vector2u> wallsCoords;
	sf::Vector2u endOfLevelCoords;
	for (unsigned int i = 0; i < objectsInLevel.y; ++i) // rows, i.e. y
	{
		std::getline(levelsReader, lineRead);
		std::stringstream ss;
		ss << lineRead;
		for (unsigned int j = 0; j < objectsInLevel.x; ++j) // cols, i.e. x
		{
			std::string typeOfObjStr;
			ss >> typeOfObjStr;
			objTypeOnLevelMap typeOfObjInt = static_cast<objTypeOnLevelMap>(std::stoi(typeOfObjStr));
			switch (typeOfObjInt)
			{
				case objTypeOnLevelMap::WALL:
					wallsCoords.push_back(sf::Vector2u(j, i));
				break;
				case objTypeOnLevelMap::END_OF_LEVEL:
					endOfLevelCoords = sf::Vector2u(i, j);
				break;
				default:
				break;
			}
		}
	}

	levelsReader.close();

	std::vector<OBJ_TYPE> resCommands;

	gameObjects.push_back(new Background(0, 0, windowDimensions.w, windowDimensions.h, false));
	resCommands.push_back(OBJ_TYPE::BACKGROUND_TYPE);

	// initialize Zero AFTER background in order not to draw background after Zero
	unsigned int zeroSpeed = 10; // TODO move this as setting or smth similar
	double zeroWidth = getGameObjSize().x / 2;
	double zeroHeight = getGameObjSize().y / 2;
	gameObjects.push_back(new ZeroCharacter(0, 0, zeroWidth, zeroHeight, zeroSpeed, false));
	resCommands.push_back(OBJ_TYPE::ZERO_TYPE);

	// init walls
	for (auto wallCoord : wallsCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(wallCoord);
		gameObjects.push_back(new Wall(worldCoords.x, worldCoords.y, getGameObjSize().x, getGameObjSize().y, false));
	}
	resCommands.push_back(OBJ_TYPE::WALL_TYPE);

	// init end of level
	sf::Vector2u eolWorldCoords = calcWorldCoordsFromMapCoords(endOfLevelCoords);
	gameObjects.push_back(new EndOfLevel(eolWorldCoords.x, eolWorldCoords.y, getGameObjSize().x, getGameObjSize().y, false));
	resCommands.push_back(OBJ_TYPE::END_OF_LEVEL_TYPE);

	// Menu load
	imagesNames[OBJ_TYPE::MENU_BUTTON_TYPE] = "MenuButtonEmptyWide.png";
	resCommands.push_back(OBJ_TYPE::MENU_BUTTON_TYPE);
	imagesNames[OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE] = "frame-transparent.png";
	resCommands.push_back(OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE);
	imagesNames[OBJ_TYPE::MENU_TYPE] = "MenuBackgroundRectangle.png";
	menus[RUN_MENU_STATE::MAIN_MENU_STATE] =
		new MainMenu(0, 0, windowDimensions.w, windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::MENU_TYPE);

	imagesNames[OBJ_TYPE::START_SCREEN_TYPE] = "StartScreen.png";
	menus[RUN_MENU_STATE::START_SCREEN_STATE] =
		new StartScreen(0, 0, windowDimensions.w, windowDimensions.h, false);
	resCommands.push_back(OBJ_TYPE::START_SCREEN_TYPE);

	imagesNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE] = "StartScreen.png";
	menus[RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE] =
		new StartScreen(0, 0, windowDimensions.w, windowDimensions.h, false);
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
		textures[command] = texture;
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
			(windowDimensions.w / objectsInLevel.x), 
			(windowDimensions.h / objectsInLevel.y));
}

sf::Vector2u ResourcesManager::calcWorldCoordsFromMapCoords(const sf::Vector2u& mapCoords)
{
	sf::Vector2u sizeOfObj(sf::Vector2u((windowDimensions.w / objectsInLevel.x), (windowDimensions.h / objectsInLevel.y)));

	sf::Vector2u result(sf::Vector2u((sizeOfObj.x * mapCoords.x), (sizeOfObj.y * mapCoords.y)));

	return result;
}

sf::Texture ResourcesManager::getTexture(Definitions::ObjectType command)
{
	return textures[command];
}

bool ResourcesManager::getAnimation(Definitions::ObjectType command, Animation& animation)
{
	bool rc = false;
	for(auto it = animations.begin(); it != animations.end(); ++it)
	{
		if (it->first == command)
		{
			animation = animations[command];
			rc = true;
			break;
		}
	}

	return rc;
}