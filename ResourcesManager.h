#pragma once
#include <unordered_map>		// map of <command, texture>
#include <vector>				// vector of commands
#include <fstream>				// read from file "levels" which textures to load
#include <string>				// strings manipulations while reading data 
#include <sstream>				// a string line read from file to be converted to ints

#include <SFML\Graphics.hpp>	// sf::Texture, sf::Sprite

#include "Definitions.h"		// LoadResourcesCommands, Animations

class ResourcesManager
{
public:
	static ResourcesManager* getInstance();
	void		loadResources(std::vector<LoadResourcesCommands> commands, int level);
	sf::Texture getTexture(LoadResourcesCommands command);
	Animation	getAnimation(LoadResourcesCommands command);
	void		releaseResources();
	~ResourcesManager();
private:
	ResourcesManager() {};
	ResourcesManager(ResourcesManager const&);
	void operator= (ResourcesManager const&);
	static ResourcesManager* instance;
	std::unordered_map<LoadResourcesCommands, sf::Texture> textures;
	std::unordered_map<LoadResourcesCommands, Animation> animations;
	Animation getAnimationFromString(std::string strData);
};