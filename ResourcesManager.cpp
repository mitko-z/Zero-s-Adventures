#include "ResourcesManager.h"

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

void ResourcesManager::loadResources(std::vector<LoadResourcesCommands> commands, int level)
{
	std::string loadPath("Data/info.dat");
	std::ifstream levelsReader(loadPath);

	if (!levelsReader)
	{
		std::string throwMessage = "Cannot load image " + loadPath;
		throw throwMessage;
	}

	std::string lineRead;
	std::vector<std::string> imagesNames;
	std::getline(levelsReader, lineRead);	// read "; name of Zero's texture.."
	std::getline(levelsReader, lineRead);	// read the name of the texture
	imagesNames.push_back(lineRead + ".png");
	std::getline(levelsReader, lineRead);	// read "; Zero texture frames x, y"
	std::getline(levelsReader, lineRead);	// read the frames upon x and y
	animations[LoadResourcesCommands::ZERO] = getAnimationFromString(lineRead);
	std::getline(levelsReader, lineRead);	// read "; number of levels"
	std::getline(levelsReader, lineRead);	// read the number of levels
	std::getline(levelsReader, lineRead);	// read "; level no."
	std::getline(levelsReader, lineRead);	// read 1
	std::getline(levelsReader, lineRead);	// read "; name for the texture of the background"
	std::getline(levelsReader, lineRead);	// read the name for the texture of the background
	imagesNames.push_back(lineRead + ".png");

	levelsReader.close();

	for (auto command : commands)
	{
		sf::Texture texture;
		std::string loadPath("Data/Images/");
		switch (command)
		{
			case LoadResourcesCommands::ZERO:
				loadPath += imagesNames[0];
			break;
			case LoadResourcesCommands::BACKGROUND:
				loadPath += imagesNames[1];
			break;
			default:
			break;
		}
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

sf::Texture ResourcesManager::getTexture(LoadResourcesCommands command)
{
	return textures[command];
}

bool ResourcesManager::getAnimation(LoadResourcesCommands command, Animation& animation)
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