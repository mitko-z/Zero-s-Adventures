#include "Game.h"

Game::Game() :
	window(sf::VideoMode(800, 600), "Zero's Adventures"),
	timePauseBetweenLevels(1000),
	timeElapsedBetweenLevels(0),
	mode(Mode::gameMode),
	runningMenuState(RunningMenuStates::MainMenu)
{
	initialize();
	loadContent();
}

void Game::initialize()
{
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	mode = Mode::menuMode;
	runningMenuState = RunningMenuStates::MainMenu;

	gameObjects.push_back(new Background(0, 0, window.getSize().x, window.getSize().y, false));
	gameObjects.push_back(new ZeroCharacter(10, 10, 10, 10, false));

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

	std::vector<LoadResourcesCommands> resCommands;
	for (auto gameObject : gameObjects)
	{
		LoadResourcesCommands resCommand = gameObject->getLoadResourcesCommand();
		if (resCommand != LoadResourcesCommands::NONE)
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
			mode = Mode::exitMode;
		}
		else
		{
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					keysPressed[event.key.code] = true;
				break;
				case sf::Event::KeyReleased:
					keysPressed[event.key.code] = false;
				break;
				default:
				break;
			}
		}
	}
	for (auto gameObject : gameObjects)
	{
		gameObject->updateEvents(keysPressed);
	}
}

void Game::update()
{
	for (auto gameObject : gameObjects)
	{
		gameObject->update();
	}
}

void Game::draw()
{

	window.clear();

	extern ResourcesManager *resMan;
	for (auto gameObject : gameObjects)
	{
		gameObject->draw(window);
	}

	window.display();
}

void Game::run()
{
	while (mode != Mode::exitMode)
	{
		eventsCapture();
		update();
		draw();
	}
}