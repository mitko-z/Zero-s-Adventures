#include "Game.h"
#include "ResourcesManager.h"

extern ResourcesManager *resMan = ResourcesManager::getInstance();

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch (std::string ex)
	{
		std::cerr << ex;
	}
	return 0;
}