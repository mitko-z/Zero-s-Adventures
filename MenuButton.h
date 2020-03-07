#pragma once

#include "GameObject.h"
#include "Definitions.h"

class MenuButton : public GameObject
{
public:
	MenuButton() : GameObject(0, 0, 0, 0, false) {}
	MenuButton(
		double x,
		double y,
		double w,
		double h,
		bool isAnimating,
		std::string text);

	void loadContent() override;
	Definitions::LoadResourcesCommands getLoadResourcesCommand() override;
	void update() override;
	void draw(sf::RenderWindow &window) override;

	bool pressed() { return isPressed_; }
	void press();
	bool isActive() { return isActive_; }
	void activate() { isActive_ = true; }
	void deactivate() { isActive_ = false; }
private:
	bool isPressed_;
	bool isActive_;
	Definitions::RunningGameStates runningGameState_;
	std::string textToDisplay;
	sf::Font font;
	sf::Text text;
};