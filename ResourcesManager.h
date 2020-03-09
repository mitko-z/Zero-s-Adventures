#pragma once
#include <unordered_map>		// map of <command, texture>
#include <vector>				// vector of commands
#include <fstream>				// read from file "levels" which textures to load
#include <string>				// strings manipulations while reading data 
#include <sstream>				// a string line read from file to be converted to ints

#include <SFML\Graphics.hpp>	// sf::Texture, sf::Sprite

#include "Definitions.h"		// LoadResourcesCommand, Animations

class ResourcesManager
{
public:
	static ResourcesManager* getInstance();
	void		loadResources(std::vector<Definitions::LoadResourcesCommand> commands, int level);
	sf::Texture getTexture(Definitions::LoadResourcesCommand command);
	bool        getAnimation(Definitions::LoadResourcesCommand command, Animation& animation);
	void		releaseResources();
	~ResourcesManager();
private:
	ResourcesManager() {};
	ResourcesManager(ResourcesManager const&);
	void operator= (ResourcesManager const&);
	static ResourcesManager* instance;
	UMAP<LOAD_RES_COMMAND, sf::Texture> textures;
	UMAP<LOAD_RES_COMMAND, Animation> animations;
	Animation getAnimationFromString(std::string strData);
};