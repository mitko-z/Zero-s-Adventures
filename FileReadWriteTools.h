#pragma once

#include <string>
#include <fstream>				// read from file "levels" which textures to load
#include <map>
#include <unordered_map>

#include "Definitions.h"

class FileReadWriteTools
{
public:

	static void extractFileCommandsAndResources(const std::string& filePath, std::map<std::string, std::string>& fileCommandsAndResources);
	static void extractFileCommandsAndResources(const std::string& filePath, std::map<std::string, std::string>& fileCommandsAndResources, int levelNum);
	static void extractSoundsFileNames(	const std::string& filePath,
										const OBJ_TYPE & gameObjectType,
										umapTypeVecStrings & soundsNames,
										umapTypeVecInts& soundsRanges,
										std::string commandForNumOfSounds = "numberOfSounds");
	template <typename T>
	static void writeToFile(const std::string& filePath, const T value, bool append = false)
	{
		auto methodForWriting = append ? std::ios::app : std::ios::trunc;
		std::ofstream fileWriter;
		fileWriter.open(filePath, std::ios::out | methodForWriting);
		if (fileWriter.is_open())
		{
			fileWriter << value;
			fileWriter.close();
		}
		else
		{
			throw "Failed to open the file for appending.";
		}
	}
private:
	static std::ifstream getReader(std::string filePath);
};