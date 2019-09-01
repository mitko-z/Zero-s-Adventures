#pragma once

#include <string>		// loading path

#include "GameObject.h"


class ZeroCharacter : public GameObject
{
public:
	ZeroCharacter() = default;
	ZeroCharacter(double x, double y, double w, double h);

	void loadContent() override;
	LoadResourcesCommands getLoadResourcesCommand() override;
	void initialize() override;
	//void updateEvents(const std::unordered_map<sf::Keyboard::Key, bool> &controllingKeys) override;
	void update() override;
	void draw(sf::RenderWindow &window) override;
private:
	double speed;
};