#include "ResourcesManager.h"

#include <sstream>

#include "ZeroCharacter.h"
#include "Background.h"
#include "MainMenu.h"
#include "StartScreen.h"
#include "Wall.h"
#include "EndOfLevel.h"
#include "FinishedLevelScreen.h"
#include "GameOverScreen.h"
#include "FinalScreen.h"
#include "Monster.h"
#include "Health.h"
#include "Weapon.h"

// file reading defines
#define GENERAL_INFO_FILE_PATH		"Data/general_info.dat"
#define WALLS_INFO_FILE_PATH		"Data/walls_info.dat"
#define BACKGROUND_INFO_FILE_PATH	"Data/background_info.dat"
#define ZERO_INFO_FILE_PATH			"Data/zero_info.dat"
#define MONSTERS_INFO_FILE_PATH		"Data/monsters_info.dat"
#define WEAPONS_INFO_FILE_PATH		"Data/weapons_info.dat"
#define END_OF_LEVEL_INFO_FILE_PATH "Data/end_of_level_info.dat"
#define HEALTH_INFO_FILE_PATH		"Data/health_info.dat"
#define MENUS_INFO_FILE_PATH		"Data/menus_info.dat"
#define IMAGES_FOLDER				"Data/Images/"
#define SOUNDS_FOLDER				"Data/Audio/sounds/"
#define AUDIO_FOLDER				"Data/Audio/"

// TODO - move these below to a settings file (when one is available)
#define START_SCREEN_SECONDS_TO_WAIT			0
#define FINISHED_LEVEL_SCREEN_SECONDS_TO_WAIT	2
#define GAME_OVER_SCREEN_SECONDS_TO_WAIT		5

ResourcesManager* ResourcesManager::m_instance = nullptr;

ResourcesManager* ResourcesManager::getInstance()
{
	if (!m_instance)
	{
		m_instance = new ResourcesManager;
	}
	return m_instance;
}

void ResourcesManager::initialize()
{
	m_gameObjects.clear();
	m_menus.clear();
	m_textures.clear();
	m_animations.clear();
}

ResourcesManager::~ResourcesManager()
{
	delete m_instance;
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
	std::vector<OBJ_TYPE> resCommands;
	umapTypeVecStrings soundNames;
	umapTypeVecInts soundsRanges;
	umapTypeString musicBackgroundNames;

	loadLevel(level, imagesNames, resCommands, soundNames, soundsRanges);

	// Menu load
	if (level == 1)
	{
		loadMenus(imagesNames, musicBackgroundNames);
		initMenus(resCommands, musicBackgroundNames);
	}

	for (auto command : resCommands)
	{
		sf::Texture texture;
		std::string loadPath(IMAGES_FOLDER);
		loadPath += imagesNames[command];
		if (!texture.loadFromFile(loadPath))
		{
			std::string throwMessage = "Cannot load image " + loadPath;
			throw throwMessage;
		}
		m_textures[command] = texture;
	} // end for each command

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
								 umapTypeString& imagesNames,
								 std::vector<OBJ_TYPE>& resCommands,
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
	resCommands.clear();
	soundsNames.clear();
	m_animations.clear();

	/// reading infos
	// read zero info
	double zeroSpeed, zeroHealth, zeroAttcackingSpeed, zeroFiringAccurracy;
	getZeroInfo(imagesNames, soundsNames, soundsRanges, zeroSpeed, zeroHealth, zeroAttcackingSpeed, zeroFiringAccurracy);

	// read background info
	getBackgroundInfo(level, imagesNames);

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

	/// initialize objects
	unsigned int numbersOfLevels = 0;
	std::vector<sf::Vector2u> wallsCoords;
	sf::Vector2u endOfLevelCoords;
	std::vector<sf::Vector2u> monstersCoords;
	std::vector<sf::Vector2u> weaponsCoords;
	getGeneralInfo(level, numbersOfLevels, wallsCoords, endOfLevelCoords, monstersCoords, weaponsCoords);

	m_gameObjects.push_back(new Background(0, 0, m_windowDimensions.w, m_windowDimensions.h, false));
	resCommands.push_back(OBJ_TYPE::BACKGROUND_TYPE);

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
	resCommands.push_back(OBJ_TYPE::ZERO_TYPE);
	dynamic_cast<ZeroCharacter*>(m_gameObjects[1])->setWeapon(ZeroCurrentWeapon);

	// init walls
	for (auto wallCoord : wallsCoords)
	{
		sf::Vector2u worldCoords = calcWorldCoordsFromMapCoords(wallCoord);
		m_gameObjects.push_back(new Wall(worldCoords.x, worldCoords.y, getGameObjSize().x, getGameObjSize().y, false));
	}
	resCommands.push_back(OBJ_TYPE::WALL_TYPE);

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
		resCommands.push_back(OBJ_TYPE::MONSTER_TYPE);
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
		resCommands.push_back(type);
	}
	for (auto& type : allProjectilesTypes)
	{
		resCommands.push_back(type);
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
	resCommands.push_back(OBJ_TYPE::END_OF_LEVEL_TYPE);

	// init health
	resCommands.push_back(OBJ_TYPE::HEALTH_TYPE);
	resCommands.push_back(OBJ_TYPE::HEALTH_BACKGROUND_TYPE);
}

void ResourcesManager::loadMenus(umapTypeString& imagesNames, umapTypeString& musicNames)
{
	std::ifstream infoReader = getReader(MENUS_INFO_FILE_PATH);
	std::string lineRead;
	std::getline(infoReader, lineRead);	// read "; name of the buttons texture (without the extension of the file)"
	std::getline(infoReader, lineRead);	// read the name of the texture for buttons
	imagesNames[OBJ_TYPE::MENU_BUTTON_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; name of the buttons highlighter texture"
	std::getline(infoReader, lineRead);	// read name of the texture for the highlighter
	imagesNames[OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; name of the menus background texture"
	std::getline(infoReader, lineRead);	// read the name of the texture for the background
	imagesNames[OBJ_TYPE::MENU_TYPE] = lineRead + ".png";
	std::getline(infoReader, lineRead);	// read "; name of the background music for menus"
	std::getline(infoReader, lineRead);	// read "; name of the music file
	musicNames[OBJ_TYPE::MENU_TYPE] = lineRead;

	std::getline(infoReader, lineRead);	// read "; name of the start screen texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::START_SCREEN_TYPE] = lineRead + ".png";

	std::getline(infoReader, lineRead);	// read "; name of the finished level screen texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE] = lineRead + ".png";

	std::getline(infoReader, lineRead);	// read "; name of the game over screen texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::GAME_OVER_SCREEN_TYPE] = lineRead + ".png";

	std::getline(infoReader, lineRead);	// read "; name of the final screen texture"
	std::getline(infoReader, lineRead);	// read "; name of the texture
	imagesNames[OBJ_TYPE::FINAL_SCREEN] = lineRead + ".png";
}

void ResourcesManager::initMenus(std::vector<OBJ_TYPE>& resCommands, umapTypeString& musicNames)
{
	resCommands.push_back(OBJ_TYPE::MENU_BUTTON_TYPE);
	resCommands.push_back(OBJ_TYPE::BUTTON_HIGHLIGHTER_TYPE);
	resCommands.push_back(OBJ_TYPE::MENU_TYPE);
	m_menus[RUN_MENU_STATE::MAIN_MENU_STATE] =
		new MainMenu(0, 0, m_windowDimensions.w, m_windowDimensions.h, false, AUDIO_FOLDER + musicNames[OBJ_TYPE::MENU_TYPE]);
	resCommands.push_back(OBJ_TYPE::START_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::START_SCREEN_STATE] =
		new StartScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false, START_SCREEN_SECONDS_TO_WAIT);
	resCommands.push_back(OBJ_TYPE::FINISHED_LEVEL_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::FINISHED_LEVEL_SCREEN_STATE] =
		new FinishedLevelScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false, FINISHED_LEVEL_SCREEN_SECONDS_TO_WAIT);
	resCommands.push_back(OBJ_TYPE::GAME_OVER_SCREEN_TYPE);
	m_menus[RUN_MENU_STATE::GAME_OVER_SCREEN_STATE] = 
		new GameOverScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false, GAME_OVER_SCREEN_SECONDS_TO_WAIT);
	resCommands.push_back(OBJ_TYPE::FINAL_SCREEN);
	m_menus[RUN_MENU_STATE::FINAL_SCREEN_STATE] =
		new FinalScreen(0, 0, m_windowDimensions.w, m_windowDimensions.h, false, GAME_OVER_SCREEN_SECONDS_TO_WAIT);
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

void ResourcesManager::getZeroInfo(umapTypeString& imagesNames, 
								   umapTypeVecStrings& soundsNames,
								   umapTypeVecInts& soundsRanges,
								   double& zeroSpeed, 
								   double& zeroHealth, 
								   double& zeroAttcackingSpeed, 
								   double& zeroFiringAccurracy)
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
	std::getline(infoReader, lineRead);	// read ; Zero attacking speed (in seconds)
	std::getline(infoReader, lineRead);	// read Zero's attacking speed
	zeroAttcackingSpeed = std::stod(lineRead);
	std::getline(infoReader, lineRead);	// read ; Zero firing accurracy (in percentages)
	std::getline(infoReader, lineRead);	// read Zero's firing accurracy
	zeroFiringAccurracy = std::stod(lineRead);
	readFromFileSoundsFileNames(infoReader, OBJ_TYPE::ZERO_TYPE, soundsNames, soundsRanges);
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
		monsterType = static_cast<MONSTERS_TYPE>(std::stoi(lineRead));
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
		std::getline(infoReader, lineRead);	// read ; Number of projectiles types monster immune from
		std::getline(infoReader, lineRead);	// read the number of projectiles types monster immune from
		int numProjTypes = std::stoi(lineRead);
		std::getline(infoReader, lineRead);	// read ; Monster immune from projectiles
		std::getline(infoReader, lineRead);	// read the list of projectiles which monster is immune of
		std::stringstream ss;
		ss << lineRead;
		std::string projectileTypeAsStr;
		for(int i = 0; i < numProjTypes; i++)
		{
			ss >> projectileTypeAsStr;
			monsterImmuneFrom.push_back(static_cast<OBJ_TYPE>(std::stoi(projectileTypeAsStr)));
		}
		readFromFileSoundsFileNames(infoReader, OBJ_TYPE::MONSTER_TYPE, soundsNames, soundsRanges);
	} while (currentLevel != level);
	infoReader.close();
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
		std::getline(infoReader, lineRead);	// read the index number for the type of weapon 
		OBJ_TYPE weaponType = static_cast<OBJ_TYPE>(std::stoi(lineRead) + static_cast<int>(OBJ_TYPE::WEAPONS_TYPES_START));
		bool noWeapon = (weaponType == OBJ_TYPE::WEAPONS_TYPES_START);
		if(!noWeapon) weaponsTypes.push_back(weaponType);
		std::getline(infoReader, lineRead);	// read ; name for the texture of the weapon (without the extension of the file)
		std::getline(infoReader, lineRead);	// read the name for the texture of the weapon
		if (!noWeapon) imagesNames[weaponType] = lineRead + ".png";
		std::getline(infoReader, lineRead);	// read ; Weapon texture frames y, x
		std::getline(infoReader, lineRead);	// read the Weapon animation texture frames
		if (!noWeapon) m_animations[weaponType] = getAnimationFromString(lineRead);
		std::getline(infoReader, lineRead);	// read ; firing rate of the weapon (in seconds)
		std::getline(infoReader, lineRead);	// read the firing rate 
		firingRate = std::stod(lineRead);
		std::getline(infoReader, lineRead);	// read ; index number for the type of projectile for this level
		std::getline(infoReader, lineRead);	// read the type of projectile
		OBJ_TYPE projectilesType = static_cast<OBJ_TYPE>(std::stoi(lineRead) + static_cast<int>(OBJ_TYPE::PROJECTILES_TYPES_START));
		if (!noWeapon) projectilesTypes.push_back(projectilesType);
		std::getline(infoReader, lineRead);	// read ; name for the texture of the projectile (without the extension of the file)
		std::getline(infoReader, lineRead);	// read the name for the texture of the projectile
		if (!noWeapon) imagesNames[projectilesType] = lineRead + ".png";
		std::getline(infoReader, lineRead);	// read ; projectiles damage
		std::getline(infoReader, lineRead);	// read the projectiles damage
		projectilesDamage = std::stod(lineRead);
		std::getline(infoReader, lineRead);	// read ; projectiles speed
		std::getline(infoReader, lineRead);	// read the projectiles speed
		projectilesSpeed = std::stod(lineRead);
		readFromFileSoundsFileNames(infoReader, weaponType, soundsNames, soundsRanges);
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

void ResourcesManager::getWeaponInfo(const unsigned int & level, UMAP<OBJ_TYPE, std::string>& imagesNames)
{
	std::ifstream infoReader = getReader(WEAPONS_INFO_FILE_PATH);
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
									  std::vector<sf::Vector2u>& monstersCoords,
									  std::vector<sf::Vector2u>& weaponsCoords)
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
		weaponsCoords.clear();

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
	} while (currentLevel != level);

	generalInfoReader.close();

	setSpeedFactor();
}

void ResourcesManager::setSpeedFactor()
{
	m_speedFactor.x = getLevelBlockDimensions().x / m_speedFactroDivider;
	m_speedFactor.y = getLevelBlockDimensions().y / m_speedFactroDivider;
}

void ResourcesManager::readFromFileSoundsFileNames(std::ifstream & fileReader, const OBJ_TYPE & gameObjectType, umapTypeVecStrings & soundsNames, umapTypeVecInts& soundsRanges)
{
	std::string lineRead;
	std::getline(fileReader, lineRead);	// read ; number of sounds
	std::getline(fileReader, lineRead);	// read how many sounds the object can make
	int numSounds = std::stoi(lineRead);
	if (numSounds > 0)
	{
		if (soundsNames.find(gameObjectType) == soundsNames.end())
		{
			soundsNames[gameObjectType] = std::vector<std::string>();
		}
		else
		{
			// clear any sounds loaded from the previous level
			soundsNames[gameObjectType].clear();
		}
		std::getline(fileReader, lineRead);	// read ; list of sounds file names
		for (int i = 0; i < numSounds; ++i)
		{
			std::getline(fileReader, lineRead);	// read the file name of the sound
			soundsNames[gameObjectType].push_back(lineRead);
		}
		std::getline(fileReader, lineRead); // read ; list of numbers sounds ranges
		std::getline(fileReader, lineRead); // read the numbers sounds ranges
		int numRanges = std::stoi(lineRead);
		std::getline(fileReader, lineRead); // read ; list of sounds ranges
		for (int i = 0; i < numRanges; ++i)
		{
			std::getline(fileReader, lineRead);
			soundsRanges[gameObjectType].push_back(std::stoi(lineRead));
		}
	}
}

const sf::Vector2f ResourcesManager::getLevelBlockDimensions()
{
	return sf::Vector2f(m_windowDimensions.w / m_objectsInLevel.x, m_windowDimensions.h / m_objectsInLevel.y);
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