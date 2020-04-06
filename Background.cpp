#include "Background.h"

void Background::loadContent()
{
	GameObject::loadContent();
	scaleSpriteTo(rect.w, rect.h, drawingObject.texture, drawingObject.sprite);
}

Definitions::LoadResourcesCommand Background::getLoadResourcesCommand()
{
	return Definitions::LoadResourcesCommand::BACKGROUND_RES;
}