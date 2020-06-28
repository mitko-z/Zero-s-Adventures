#include "Game.h"
#include "ResourcesManager.h"

extern ResourcesManager *resMan = ResourcesManager::getInstance();

int main()
{
	try
	{
		Game game;
		game.initialize();
		game.loadContent();
		game.run();
	}
	catch (std::string ex)
	{
		std::cerr << ex;
		std::getchar();
	}
	return 0;
}