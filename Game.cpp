#include <memory>
#include "Game.h"


#include "ResourcesManager.h"
#include "Definitions.h"
#include "StateMachine.h"
#include "BackgroundAudioPlayer.h"
#include "FileReadTools.h"

Game::Game() :
	m_window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Zero's Adventures", sf::Style::Fullscreen),
	m_currentLevel(1)
{
	FileReadTools::writeToFile("game_is_running.inf", 1); // warn the external world that the game has been started
} 

Game::~Game()
{
	FileReadTools::writeToFile("game_is_running.inf", 0); // say to the external world that the game has been ended
}

void Game::initialize()
{
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	stateMachine->initialize();

	m_window.setFramerateLimit(60);
	m_window.setKeyRepeatEnabled(false);

	extern std::shared_ptr <ResourcesManager> resMan;
	resMan->setWindowDimensions(m_window.getSize().x, m_window.getSize().y);
}

void Game::loadContent()
{
	extern std::shared_ptr <ResourcesManager> resMan;
	resMan->loadResources(m_currentLevel);

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
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	while (m_window.pollEvent(m_event))
	{
		if (m_event.type == sf::Event::KeyPressed && m_event.key.code == sf::Keyboard::Escape)
		{
			if (stateMachine->getMode() == MODE::GAME_MODE)
			{
				stateMachine->setEventByGameCommand(COMMAND::MENU_COMMAND);
				stateMachine->setEventByGameCommand(COMMAND::MIAN_MENU_COMMAND);
			}
			else
			{
				stateMachine->setEventByGameCommand(COMMAND::GAME_COMMAND);
			}
		}
		else if (m_event.type == sf::Event::Closed)
		{
			stateMachine->setEventByGameCommand(COMMAND::EXIT_COMMAND);
		}
		else
		{
			switch (m_event.type)
			{
				case sf::Event::KeyPressed:
					stateMachine->addPressedKey(m_event.key.code);
				break;
				case sf::Event::KeyReleased:
					stateMachine->addReleasedKey(m_event.key.code);
				break;
				default:
				break;
			}
		}
	}
#pragma endregion

#pragma region update events of game objects
	extern std::shared_ptr <ResourcesManager> resMan;
	switch (stateMachine->getMode())
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
			menus[stateMachine->getRunningMenuState()]->updateEvents();
		}
		break;
		case MODE::NEXT_LEVEL_MODE:
			m_currentLevel++;
			loadContent();
			stateMachine->setEventByGameCommand(COMMAND::GAME_COMMAND);
		break;
		case MODE::INITIALIZE_MODE:
			stateMachine->initialize();
			m_currentLevel = 1;
			loadContent();
		break;
		default:
		break;
	}
#pragma endregion

	// clear the events
	stateMachine->nullEvents();
}

void Game::processColisions()
{
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	extern std::shared_ptr <ResourcesManager> resMan;
	switch (stateMachine->getMode())
	{
		case MODE::GAME_MODE:
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
		case MODE::MENU_MODE:
		{
			// TODO - mouse pointer "colisions" with buttons
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
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	extern std::shared_ptr <ResourcesManager> resMan;
	switch (stateMachine->getMode())
	{
		case MODE::GAME_MODE:
		{
			std::vector<GameObject *> gameObjects = resMan->getGameObjects();
			for (auto gameObject : gameObjects)
			{
				gameObject->update();
			}
			resMan->removeInactiveGameObjects();
		}
		break;
		case MODE::MENU_MODE:
		{
			UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
			menus[stateMachine->getRunningMenuState()]->update();
		}
		break;
		default:
		break;
	}
}

void Game::draw()
{
	m_window.clear();

	extern std::shared_ptr <ResourcesManager> resMan;
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	switch (stateMachine->getMode())
	{
		case Definitions::Mode::GAME_MODE:
		{
			std::vector<GameObject *> gameObjects = resMan->getGameObjects();
			for (auto gameObject : 
				gameObjects)
			{
				gameObject->draw(m_window);
			}
		}
		break;
		case Definitions::Mode::MENU_MODE:
		{
			UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
			menus[stateMachine->getRunningMenuState()]->draw(m_window);
		}
		break;
		default:
		break;
	}
	m_window.display();
}

void Game::playAudio()
{
	extern std::shared_ptr <ResourcesManager> resMan;
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	switch (stateMachine->getMode())
	{
	case Definitions::Mode::GAME_MODE:
	{
		auto gameObjects = resMan->getGameObjects();
		for (auto& gameObj : gameObjects)
		{
			gameObj->playAudio();
		}
	}
	break;
	case Definitions::Mode::MENU_MODE:
	{
		UMAP<RUN_MENU_STATE, Menu*> menus = resMan->getMenus();
		menus[stateMachine->getRunningMenuState()]->playAudio();
	}
	break;
	default:
		break;
	}
}

void Game::run()
{
	std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
	while (stateMachine->getMode() != Definitions::Mode::EXIT_MODE)
	{
		eventsCapture();
		update();
		processColisions();
		draw();
		playAudio();
	}
}