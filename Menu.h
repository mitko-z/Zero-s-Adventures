#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "ButtonHighlighter.h"
#include <vector>

class Menu : public GameObject
{
public:
	Menu() : GameObject(0, 0, 0, 0, false) {}
	Menu(double x, double y, double w, double h, bool isAnimating, std::string backgroundMusicFilename) :
		m_highlighter(10, 10, 10, 10, false),
		m_backgroundMusicFilename(backgroundMusicFilename),
		GameObject(x, y, w, h, isAnimating)
	{
	}

	virtual void initialize() override;
	virtual void loadContent() override;
	Definitions::ObjectType getType() override;
	virtual void update() override;
	virtual void draw(sf::RenderWindow &window) override;
	void playAudio() override;
	virtual void playBackgroundMusic();
protected:
	void activatePrevButton();
	void activateNextButton();
	void updateKeys(const MAP_KEYS& keysPressed, const MAP_KEYS& keysReleased) override;
	void setAndInsertButtons(const std::vector<BUTTON_TYPE>& buttonTypes);

	std::vector<MenuButton*> m_buttons;
	ButtonHighlighter m_highlighter;
	int m_activeButtonIndex;
	std::string m_backgroundMusicFilename;
private:
	void setHighlighter(size_t buttonIndex);

	enum menuSoundTypes
	{
		MENU_SOUND_TYPE_NEXT_BUTTON = 0,
	};
};