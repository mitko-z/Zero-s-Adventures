#include "AudioPlayer.h"

sf::Music AudioPlayer::player;

std::shared_ptr<AudioPlayer> AudioPlayer::getInstance()
{
	static std::shared_ptr<AudioPlayer> instance{ new AudioPlayer };
	return instance;
}

void AudioPlayer::initialize(std::string pathToFile)
{
	player.openFromFile(pathToFile);
	player.setLoop(true);
}

void AudioPlayer::play()
{
	if(player.getStatus() != sf::SoundSource::Playing)
		player.play();
}

void AudioPlayer::volumeUp()
{
	// to do
}

void AudioPlayer::volumeDown()
{
	// to do
}
