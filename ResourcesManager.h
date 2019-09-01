#pragma once
#include <unordered_map>		// map of <command, texture>
#include <vector>				// vector of commands
#include <fstream>				// read from file "levels" which textures to load
#include <string>				// strings manipulations while reading data

#include <SFML\Graphics.hpp>	// sf::Texture

#include "Definitions.h"		// LoadResourcesCommands

class ResourcesManager
{
public:
	static ResourcesManager* getInstance();
	void		loadResources(std::vector<LoadResourcesCommands> commands, int level);
	sf::Texture getTexture(LoadResourcesCommands command);
	void		releaseResources();
	~ResourcesManager();
private:
	ResourcesManager() {};
	ResourcesManager(ResourcesManager const&);
	void operator= (ResourcesManager const&);
	static ResourcesManager* instance;
	std::unordered_map<LoadResourcesCommands, sf::Texture> textures;
};