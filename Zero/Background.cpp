#include "Background.h"
#include "BackgroundAudioPlayer.h"
#include "StateMachine.h"


Definitions::ObjectType Background::getType()
{
	return Definitions::ObjectType::BACKGROUND_TYPE;
}

void Background::playAudio()
{
	BackgroundAudioPlayer::getInstance()->play(m_backgroundMusicFileName);
}

std::ostringstream Background::getCurrentState()
{
	std::ostringstream oss = GameObject::getCurrentState();
	oss << addLineForOSS(m_backgroundMusicFileName, true, "Background music");

	return oss;
}
