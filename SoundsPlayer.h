#pragma once

#include <memory>
#include <vector>
#include "SFML/Audio.hpp"

class SoundsPlayer
{
public:
	static std::shared_ptr<SoundsPlayer> getInstance();
	void play(const sf::SoundBuffer& buffer);
private:
	SoundsPlayer() = default;
	std::vector<sf::Sound> m_sounds;
};