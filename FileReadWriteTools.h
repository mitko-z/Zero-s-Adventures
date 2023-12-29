#pragma once

#include <string>
#include <fstream>				// read from file "levels" which textures to load
#include <sstream>
#include <map>
#include <unordered_map>
#include <string_view>

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
	
	/// <summary>
	/// Read the content of a file
	/// </summary>
	/// <param name="filePath"> The path to file</param>
	/// <param name="skipTimeStamp"> Will skip the first line of a file (usualli it is the time stamp for a save file)</param>
	/// <returns></returns>
	static std::istringstream readFromFile(const std::string& filePath, bool skipTimeStamp = true)
	{
		std::ifstream fileReader;
		fileReader.open(filePath, std::ios::in);
		std::string lineRead;
		std::ostringstream oss;
		if (fileReader.is_open())
		{
			if (skipTimeStamp && fileReader)
			{
				std::getline(fileReader, lineRead);
			}

			while (fileReader)
			{
				std::getline(fileReader, lineRead);
				oss << lineRead << '\n';
			}
		}
		else
		{
			throw "Unable to load file " + filePath;
		}

		return std::istringstream(oss.str());
	}

	static std::string getLine(std::istringstream& stream, const std::string& source = "")
	{
		std::string result;
		if (std::getline(stream, result))
		{
			return result;
		}
		else
		{
			std::string addSourceInfo = "";
			if (source != "")
			{
				addSourceInfo = " (From source " + source + ")";
			}
			throw "Unable to read from stream" + addSourceInfo;
		}
	}

	static std::vector<std::istringstream> getGameObjectsData(std::istringstream& data)
	{
		std::string lineRead{};
		std::vector<std::istringstream> gameObjectsData;
		std::getline(data, lineRead); // we expect the first line to be "; Game object type" so we skip it initially
		while (std::getline(data, lineRead))
		{
			std::ostringstream gameObjectData;
			gameObjectData << lineRead << '\n';
			while (std::getline(data, lineRead) && lineRead != "; Game object type")
			{
				if (lineRead[0] != ';') // skip comments
				{
					gameObjectData << lineRead << '\n';
				}
			}
			gameObjectsData.push_back(std::istringstream(gameObjectData.str()));
		}
		return gameObjectsData;
	}

	template <typename T>
	static T readLine(std::istringstream& iss);

	/*template <typename T>
	static T readLine(std::istringstream& iss) 
	{
		T result;
		std::string lineRead{};
		std::getline(iss, lineRead);
		result = static_cast<T>(lineRead);
		
		return result;
	}*/
private:
	static std::ifstream getReader(std::string filePath);
};


template <>
inline double FileReadWriteTools::readLine<double>(std::istringstream& iss) {
	double result;
	std::string lineRead;
	// Use std::stod to convert the string to double
	if (!std::getline(iss, lineRead)) {
		throw "Error: Failed to convert line to double.";
	}
	result = std::stod(lineRead);
	return result;
}


template <typename T>
inline T FileReadWriteTools::readLine(std::istringstream& iss) {
	// For non-double types, convert to int using std::stoi and then cast to T
	T result;
	std::string lineRead{};
	if (!std::getline(iss, lineRead))
	{
		throw "Error: Failed to convert line to int";
	}
	result = static_cast<T>(std::stoi(lineRead));

	return result;
}