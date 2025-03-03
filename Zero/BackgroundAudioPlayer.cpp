#include "BackgroundAudioPlayer.h"

#include "StateMachine.h"

sf::Music BackgroundAudioPlayer::player;

std::shared_ptr<BackgroundAudioPlayer> BackgroundAudioPlayer::getInstance()
{
	static std::shared_ptr<BackgroundAudioPlayer> instance{ new BackgroundAudioPlayer };
	return instance;
}

void BackgroundAudioPlayer::initialize(const std::string& pathToFile, bool looped)
{
	if (!player.openFromFile(pathToFile))
	{
		std::string throwMessage = "Could not open audio file " + pathToFile;
		throw(throwMessage);
	}
	player.setLoop(looped);
}

void BackgroundAudioPlayer::play()
{
	if(player.getStatus() != sf::SoundSource::Playing)
		player.play();
}

void BackgroundAudioPlayer::play(const std::string& pathToFile, bool looped)
{
	auto stateMachine = StateMachine::getInstnce();
	if (stateMachine->toChangeAudio())
	{
		if (player.getStatus() == sf::SoundSource::Playing)
		{
			stop();
		}
		initialize(pathToFile, looped);
		if (stateMachine->toPlayAudio())
		{
			play();
		}
		stateMachine->setToChangeAudio(false);
	}
}



void BackgroundAudioPlayer::volumeUp()
{
	// to do
}

void BackgroundAudioPlayer::volumeDown()
{
	// to do
}
