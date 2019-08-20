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

	// define the pressedKeysFromKeyboard which control Zero.
	// they must be defined in pairs in strict order; one pair controls one action:
	// 0 and 1 - move up
	// 2, 3 - move down
	// 4, 5 - left
	// 6, 7 - right
	// 8, 9 - fire
	// 10, 11 - take item from the ground (which is not owned by anyone)
	// 12, 13 - leaves the item
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::W, false));		// move up
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::Up, false));		// move up
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::S, false));		// move down
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::Down, false));	// move down
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::A, false));		// move left
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::Left, false));	// move left
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::D, false));		// move right
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::Right, false));	// move right
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::Space, false));	// fire
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::LControl, false));// fire
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::F, false));		// take weapon/item
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::Return, false));	// take weapon/item
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::R, false));		// drop weapon
	controllingKeysForZero.insert(std::make_pair(sf::Keyboard::BackSpace, false));// drop weapon
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