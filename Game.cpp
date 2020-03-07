#include "Game.h"

Game::Game() :
	window(sf::VideoMode(800, 600), "Zero's Adventures"),
	timePauseBetweenLevels(1000),
	timeElapsedBetweenLevels(0),
	mode(Definitions::Mode::gameMode),
	runningMenuState(Definitions::RunningMenuStates::Menu)
{
}

void Game::initialize()
{
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	gameObjects.push_back(new Background(0, 0, window.getSize().x, window.getSize().y, false));
	gameObjects.push_back(new ZeroCharacter(10, 10, 10, 10, false));
	gameObjects.push_back(new MainMenu(0, 0, window.getSize().x, window.getSize().y, false));

	for (auto gameObject : gameObjects)
	{
		gameObject->initialize();
	}
}

void Game::loadContent()
{
	std::string loadPath;
	// fonts
	loadPath = "Data/Fonts/arial.ttf";
	if (font.loadFromFile(loadPath) == 0)
	{
		std::string throwMessage = "Cannot load font " + loadPath;
		throw throwMessage;
	}

	// texts
	sf::Text aText;
	aText.setFont(font);
	aText.setCharacterSize(window.getSize().y*0.1);
	aText.setPosition(10, 10);
	aText.setString("hgdj");

	std::vector<Definitions::LoadResourcesCommands> resCommands;
	std::vector<GameObject *> allGameObjects(gameObjects);
	allGameObjects.push_back(new MenuButton());
	allGameObjects.push_back(new ButtonHighlighter());
	for (auto gameObject : allGameObjects)
	{
		Definitions::LoadResourcesCommands resCommand = gameObject->getLoadResourcesCommand();
		if (resCommand != Definitions::LoadResourcesCommands::NONE)
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
}

void Game::eventsCapture()
{
	while (window.pollEvent(event))
	{
		if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || (event.type == sf::Event::Closed))
		{
			mode = Definitions::Mode::exitMode;
		}
		else
		{
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					eventsHolder.addPressedKey(event.key.code);
				break;
				case sf::Event::KeyReleased:
					eventsHolder.addReleasedKey(event.key.code);
				break;
				default:
				break;
			}
		}
	}
	/*for (auto gameObject : gameObjects)
	{
		gameObject->updateEvents(eventsHolder.getPressedKeys(), eventsHolder.getReleasedKeys());
	}*/
	gameObjects[2]->updateEvents(eventsHolder.getPressedKeys(), eventsHolder.getReleasedKeys());

	// clear the events
	eventsHolder.nullEvents();
}

void Game::update()
{
	/*for (auto gameObject : gameObjects)
	{
		gameObject->update();
	}*/
	gameObjects[2]->update();
}

void Game::draw()
{

	window.clear();

	extern ResourcesManager *resMan;
	/*for (auto gameObject : gameObjects)
	{
		gameObject->draw(window);
	}*/
	gameObjects[2]->draw(window);
	window.display();
}

void Game::run()
{
	while (mode != Definitions::Mode::exitMode)
	{
		eventsCapture();
		update();
		draw();
	}
}