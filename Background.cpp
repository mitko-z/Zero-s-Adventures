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
