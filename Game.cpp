#include "Game.h"

Game::Game() :
	window(sf::VideoMode(800, 600), "Zero's Adventures"),
	timePauseBetweenLevels(1000),
	timeElapsedBetweenLevels(0),
	mode(Mode::gameMode),
	runningMenuState(RunningMenuStates::MainMenu)
{
	initialize();
	try
	{
		loadContent();
	}
	catch (std::string ex)
	{
		std::cerr << ex;
	}
}

void Game::initialize()
{
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	mode = Mode::menuMode;
	runningMenuState = RunningMenuStates::MainMenu;

	gameObjects.push_back(new ZeroCharacter(10, 10, 10, 10));

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

	// images
	loadPath = "Data/Images/Background.png";
	if (!backgroundTexture.loadFromFile(loadPath))
	{
		std::string throwMessage = "Cannot load image " + loadPath;
		throw throwMessage;
	}

	// rendering shapes
	backgroundRectangle.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	backgroundRectangle.setPosition(0, 0);
	backgroundRectangle.setTexture(&backgroundTexture);

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

		switch (event.type)
		{
			case sf::Event::KeyPressed:
				if (controllingKeysForZero.find(event.key.code) != controllingKeysForZero.end())
				{
					controllingKeysForZero[event.key.code] = true;
				}
			break;
			case sf::Event::KeyReleased:
				if (controllingKeysForZero.find(event.key.code) != controllingKeysForZero.end())
				{
					controllingKeysForZero[event.key.code] = false;
				}
			break;
			default:
			break;
		}
	}
}

void Game::update()
{

}

void Game::draw()
{

	window.clear();

	window.draw(backgroundRectangle);
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