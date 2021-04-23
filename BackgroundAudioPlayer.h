#pragma once

#include "SFML\Audio.hpp"
#include <memory>
#include <string>

class BackgroundAudioPlayer
{
public:
	static std::shared_ptr<BackgroundAudioPlayer> getInstance();

	void initialize(const std::string& pathToFile);
	void play();
	void play(const std::string& pathToFile);
	void stop() { player.stop(); }
	void volumeUp();
	void volumeDown();
private:
	BackgroundAudioPlayer() {}
	static sf::Music player;
};