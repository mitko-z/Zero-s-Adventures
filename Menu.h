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
	Definitions::ObjectType getType() override;
	virtual void update() override;
	virtual void draw(sf::RenderWindow &window) override;

protected:
	void activatePrevButton();
	void activateNextButton();
	std::vector<MenuButton*> buttons;
	ButtonHighlighter highlighter;
	int activeButtonIndex;
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
	void setAndInsertButtons(const std::vector<Definitions::ButtonType>& buttonTypes);
private:
	void setHighlighter(size_t buttonIndex);
};