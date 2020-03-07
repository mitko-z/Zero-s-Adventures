#include "Background.h"

void Background::loadContent()
{
	GameObject::loadContent();
	sf::Vector2f factor{
		static_cast<float>(rect.w / drawingObject.texture.getSize().x),
		static_cast<float>(rect.h / drawingObject.texture.getSize().y)};
	drawingObject.sprite.scale(factor);
}

Definitions::LoadResourcesCommands Background::getLoadResourcesCommand()
{
	return Definitions::LoadResourcesCommands::BACKGROUND;
}