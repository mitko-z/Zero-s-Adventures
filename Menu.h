#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "ButtonHighlighter.h"
#include <vector>

class Menu : public GameObject
{
public:
	Menu() : GameObject(0, 0, 0, 0, false) {}
	Menu(double x, double y, double w, double h, bool isAnimating) :
		highlighter(10, 10, 10, 10, false),
		GameObject(x, y, w, h, isAnimating) 
	{
	}

	virtual void initialize() override;
	virtual void loadContent() override;
	Definitions::LoadResourcesCommands getLoadResourcesCommand() override;
	virtual void updateEvents(const UMAP<sf::Keyboard::Key, bool> &keysPressed,
		const UMAP<sf::Keyboard::Key, bool> &keysReleased) override;
	virtual void update() override;
	virtual void draw(sf::RenderWindow &window) override;

protected:
	void activatePrevButton();
	void activateNextButton();
	std::vector<MenuButton*> buttons;
	ButtonHighlighter highlighter;
	int activeButtonIndex;
	void setAndInsertButtons(const std::vector<std::string>& texts);
private:
	void setHighlighter(size_t buttonIndex);
};