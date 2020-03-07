#include "ButtonHighlighter.h"


Definitions::LoadResourcesCommands ButtonHighlighter::getLoadResourcesCommand()
{
	return Definitions::LoadResourcesCommands::BUTTON_HIGHLIGHTER;
}

void ButtonHighlighter::loadContent()
{
	GameObject::loadContent();
	scaleSpriteTo(rect.w, rect.h, drawingObject.texture, drawingObject.sprite);
}