#include <memory>
#include "Game.h"


#include "ResourcesManager.h"
#include "Definitions.h"
#include "EventsHolder.h"
#include "Background.h"
#include "ZeroCharacter.h"
#include "MainMenu.h"
#include "StartScreen.h"

Game::Game() :
	window(sf::VideoMode(800, 600), "Zero's Adventures"),
	timePauseBetweenLevels(1000),
	timeElapsedBetweenLevels(0)
{
}

void Game::initialize()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	eventsHolder->initialize();

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	gameObjects.push_back(new Background(0, 0, window.getSize().x, window.getSize().y, false));
	gameObjects.push_back(new ZeroCharacter(10, 10, 10, 10, false));
	menus[Definitions::RunningMenuState::MAIN_MENU_STATE] = 
		new MainMenu(0, 0, window.getSize().x, window.getSize().y, false);
	menus[Definitions::RunningMenuState::START_SCREEN_STATE] =
		new StartScreen(0, 0, window.getSize().x, window.getSize().y, false);

	for (auto gameObject : gameObjects)
	{
		gameObject->initialize();
	}
	for (auto it = menus.begin(); it != menus.end(); ++it)
	{
		it->second->initialize();
	}
}

void Game::loadContent()
{
	std::vector<Definitions::LoadResourcesCommand> resCommands;
	std::vector<GameObject *> allGameObjects(gameObjects);
	allGameObjects.push_back(new Menu());
	allGameObjects.push_back(new MenuButton());
	allGameObjects.push_back(new ButtonHighlighter());
	allGameObjects.push_back(new StartScreen());
	for (auto gameObject : allGameObjects)
	{
		Definitions::LoadResourcesCommand resCommand = gameObject->getLoadResourcesCommand();
		if (resCommand != Definitions::LoadResourcesCommand::NONE)
		{
			resCommands.push_back(resCommand);
		}
	}
	extern ResourcesManager *resMan;
	resMan->loadResources(resCommands, 1);

	for (auto gameObject : gameObjects)
	{
		gameObject->loadContent();
	}
	for (auto it = menus.begin(); it != menus.end(); ++it)
	{
		it->second->loadContent();
	}
}

void Game::eventsCapture()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			if (eventsHolder->getMode() == Definitions::Mode::GAME_MODE)
			{
				eventsHolder->setEventByGameCommand(Definitions::GameCommand::MENU_COMMAND);
			}
			else
			{
				eventsHolder->setEventByGameCommand(Definitions::GameCommand::GAME_COMMAND);
			}
		}
		else if (event.type == sf::Event::Closed)
		{
			eventsHolder->setEventByGameCommand(Definitions::GameCommand::EXIT_COMMAND);
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
	switch (eventsHolder->getMode())
	{
		case Definitions::Mode::GAME_MODE:
			for (auto gameObject : gameObjects)
			{
				gameObject->updateEvents();
			}
		break;
		case Definitions::Mode::MENU_MODE:
			menus[eventsHolder->getRunningMenuState()]->updateEvents();
		break;
		default:
		break;
	}

	// clear the events
	eventsHolder->nullEvents();
}

void Game::update()
{
	std::shared_ptr<EventsHolder> eventsHolder = EventsHolder::getInstnce();
	switch (eventsHolder->getMode())
	{
	case Definitions::Mode::GAME_MODE:
		for (auto gameObject : gameObjects)
		{
			gameObject->update();
		}
		break;
	case Definitions::Mode::MENU_MODE:
		menus[eventsHolder->getRunningMenuState()]->update();
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
		for (auto gameObject : gameObjects)
		{
			gameObject->draw(window);
		}
		break;
	case Definitions::Mode::MENU_MODE:
		menus[eventsHolder->getRunningMenuState()]->draw(window);
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
		draw();
	}
}