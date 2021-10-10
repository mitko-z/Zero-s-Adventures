#pragma once

#include <string>
#include <fstream>				// read from file "levels" which textures to load
#include <map>
#include <unordered_map>

#include "Definitions.h"

class FileReadTools
{
public:

	static void extractFileCommandsAndResources(const std::string& filePath, std::map<std::string, std::string>& fileCommandsAndResources);
	static void extractFileCommandsAndResources(const std::string& filePath, std::map<std::string, std::string>& fileCommandsAndResources, int levelNum);
	static void extractSoundsFileNames(	const std::string& filePath,
										const OBJ_TYPE & gameObjectType,
										umapTypeVecStrings & soundsNames,
										umapTypeVecInts& soundsRanges,
										std::string commandForNumOfSounds = "numberOfSounds");
private:
	static std::ifstream getReader(std::string filePath);
};