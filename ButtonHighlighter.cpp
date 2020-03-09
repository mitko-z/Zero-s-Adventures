#include "ButtonHighlighter.h"


Definitions::LoadResourcesCommand ButtonHighlighter::getLoadResourcesCommand()
{
	return Definitions::LoadResourcesCommand::BUTTON_HIGHLIGHTER;
}

void ButtonHighlighter::loadContent()
{
	GameObject::loadContent();
	scaleSpriteTo(rect.w, rect.h, drawingObject.texture, drawingObject.sprite);
}