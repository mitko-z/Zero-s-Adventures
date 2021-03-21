#pragma once

#include "SFML\Audio.hpp"
#include <memory>
#include <string>

class AudioPlayer
{
public:
	static std::shared_ptr<AudioPlayer> getInstance();

	void initialize(std::string pathToFile);
	void play();
	void stop() { player.stop(); }
	void volumeUp();
	void volumeDown();
private:
	AudioPlayer() {}
	static sf::Music player;
};