#include "FileReadTools.h"


std::ifstream FileReadTools::getReader(std::string filePath)
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

void FileReadTools::extractFileCommandsAndResources(const std::string& filePath, std::map<std::string, std::string>& fileCommandsAndResources)
{
	std::ifstream infoReader = getReader(filePath);
	std::string lineRead;
	size_t separatorPos;
	while (std::getline(infoReader, lineRead))
	{
		if (lineRead[0] != ';')
		{
			separatorPos = lineRead.find('=');
			fileCommandsAndResources[lineRead.substr(0, separatorPos)] = lineRead.substr(separatorPos + 1, lineRead.size() - separatorPos);
		}
	}
	infoReader.close();
}

void FileReadTools::extractFileCommandsAndResources(const std::string & filePath, std::map<std::string, std::string>& fileCommandsAndResources, int levelNum)
{
	std::ifstream infoReader = getReader(filePath);


	std::string lineRead;
	std::string command = "";
	size_t separatorPos;
	int levelRead = 0;
	const std::string levelNumCommandName = "levelNum";
	do
	{
		std::getline(infoReader, lineRead);
		if (lineRead[0] != ';')
		{
			separatorPos = lineRead.find('=');
			command = lineRead.substr(0, separatorPos);
			if (command == levelNumCommandName) levelRead = stoi(lineRead.substr(separatorPos + 1, lineRead.size() - separatorPos));
		}
	} while (levelRead != levelNum);

	while (std::getline(infoReader, lineRead))
	{
		if (lineRead[0] != ';')
		{
			separatorPos = lineRead.find('=');
			command = lineRead.substr(0, separatorPos);
			if (command == levelNumCommandName) break;

			fileCommandsAndResources[command] = lineRead.substr(separatorPos + 1, lineRead.size() - separatorPos);
		}
	}
	infoReader.close();
}


void FileReadTools::extractSoundsFileNames(	const std::string& filePath,
											const OBJ_TYPE & gameObjectType,
											umapTypeVecStrings & soundsNames,
											umapTypeVecInts& soundsRanges,
											std::string commandForNumOfSounds /*= "numberOfSounds"*/)
{
	std::ifstream fileReader = getReader(filePath);
	std::string lineRead;
	std::string command = "";
	size_t separatorPos;
	int numOfSounds = 0;
	do
	{
		std::getline(fileReader, lineRead);
		if (lineRead[0] != ';')
		{
			separatorPos = lineRead.find('=');
			command = lineRead.substr(0, separatorPos);
		}
	} while (command != commandForNumOfSounds);
	numOfSounds = std::stoi(lineRead.substr(separatorPos + 1, lineRead.size() - separatorPos));

	if (numOfSounds > 0)
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
		for (int i = 0; i < numOfSounds; ++i)
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

void FileReadTools::writeToFile(const std::string& filePath, const int value)
{
	std::ofstream fileReader;
	fileReader.open(filePath, std::ios::out | std::ios::trunc);
	fileReader << value;
	fileReader.close();
}
