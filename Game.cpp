#include <memory>
#include "Game.h"


#include "ResourcesManager.h"
#include "Definitions.h"
#include "EventsHolder.h"

Game::Game() :
	window(sf::VideoMode(800, 600), "Zero's Adventures"),
	timePauseBetweenLevels(1000),
	timeElapsedBetweenLevels(0),
	currentLevel(1)
{
}

void Game::initialize()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	eventsHolder->initialize();

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	extern ResourcesManager* resMan;
	resMan->setWindowDimensions(window.getSize().x, window.getSize().y);
}

void Game::loadContent()
{
	extern ResourcesManager *resMan;
	resMan->loadResources(currentLevel);

	std::vector<GameObject *> gameObjects = resMan->getGameObjects();
	for (auto gameObject : gameObjects)
	{
		gameObject->initialize();
		gameObject->loadContent();
	}

	UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
	for (auto it = menus.begin(); it != menus.end(); ++it)
	{
		it->second->initialize();
		it->second->loadContent();
	}
}

void Game::eventsCapture()
{
#pragma region capture gameplay events
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			if (eventsHolder->getMode() == Definitions::Mode::GAME_MODE)
			{
				eventsHolder->setEventByGameCommand(Definitions::Command::MENU_COMMAND);
			}
			else
			{
				eventsHolder->setEventByGameCommand(Definitions::Command::GAME_COMMAND);
			}
		}
		else if (event.type == sf::Event::Closed)
		{
			eventsHolder->setEventByGameCommand(Definitions::Command::EXIT_COMMAND);
		}
		else
		{
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					eventsHolder->addPressedKey(event.key.code);
				break;
				case sf::Event::KeyReleased:
					eventsHolder->addReleasedKey(event.key.code);
				break;
				default:
				break;
			}
		}
	}
#pragma endregion

#pragma region update events of game objects
	extern ResourcesManager *resMan;
	switch (eventsHolder->getMode())
	{
		case MODE::GAME_MODE:
		{
			std::vector<GameObject *> gameObjects = resMan->getGameObjects();
			for (auto gameObject : gameObjects)
			{
				gameObject->updateEvents();
			}
		}
		break;
		case MODE::MENU_MODE:
		{
			UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
			menus[eventsHolder->getRunningMenuState()]->updateEvents();
		}
		break;
		case MODE::NEXT_LEVEL_MODE:
			currentLevel++;
			loadContent();
			eventsHolder->setEventByGameCommand(COMMAND::GAME_COMMAND);
		break;
		break;
		default:
		break;
	}
#pragma endregion

	// clear the events
	eventsHolder->nullEvents();
}

void Game::processColisions()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	extern ResourcesManager *resMan;
	switch (eventsHolder->getMode())
	{
	case Definitions::Mode::GAME_MODE:
	{
		std::vector<GameObject *> gameObjects = resMan->getGameObjects();
		for (auto gameObject : gameObjects)
		{
			gameObject->nullCollisions();
		}
		size_t numOfGameObjs = gameObjects.size();
		for (size_t i = 1; i < numOfGameObjs - 1; ++i) // 0 is the background - not interacting with the other game objs
		{
			for (size_t j = i + 1; j < numOfGameObjs; ++j)
			{
				if (twoObjsColide(*gameObjects[i], *gameObjects[j]))
				{
					gameObjects[i]->setCollisionWith(*gameObjects[j]);
					gameObjects[j]->setCollisionWith(*gameObjects[i]);
				}
			}
		}
		for (auto gameObject : gameObjects)
		{
			gameObject->processCollisions();
		}
	}
	break;
	case Definitions::Mode::MENU_MODE:
	{
		// TODO
	}
	break;
	default:
		break;
	}
}

bool Game::twoObjsColide(const GameObject & obj1, const GameObject & obj2)
{
	Rectangle rect1 = obj1.getRect();
	Rectangle rect2 = obj2.getRect();
	double x1 = rect1.x; double y1 = rect1.y;
	double xw1 = x1 + rect1.w; double yh1 = y1 + rect1.h;
	double x2 = rect2.x; double y2 = rect2.y;
	double xw2 = x2 + rect2.w; double yh2 = y2 + rect2.h;
	
	if ((yh1 < y2) || (y1 > yh2))
		return false;
	if ((xw1 < x2) || (x1 > xw2))
		return false;

	return true;
}

void Game::update()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	extern ResourcesManager *resMan;
	switch (eventsHolder->getMode())
	{
		case Definitions::Mode::GAME_MODE:
		{
			std::vector<GameObject *> gameObjects = resMan->getGameObjects();
			for (auto gameObject : gameObjects)
			{
				gameObject->update();
			}
		}
		break;
		case Definitions::Mode::MENU_MODE:
		{
			UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
			menus[eventsHolder->getRunningMenuState()]->update();
		}
		break;
		default:
		break;
	}
}

void Game::draw()
{
	window.clear();

	extern ResourcesManager *resMan;
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	switch (eventsHolder->getMode())
	{
		case Definitions::Mode::GAME_MODE:
		{
			std::vector<GameObject *> gameObjects = resMan->getGameObjects();
			for (auto gameObject : gameObjects)
			{
				gameObject->draw(window);
			}
		}
		break;
		case Definitions::Mode::MENU_MODE:
		{
			UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
			menus[eventsHolder->getRunningMenuState()]->draw(window);
		}
		break;
		default:
		break;
	}
	window.display();
}

void Game::run()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	while (eventsHolder->getMode() != Definitions::Mode::EXIT_MODE)
	{
		eventsCapture();
		update();
		processColisions();
		draw();
	}
}