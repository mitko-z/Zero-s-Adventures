#include "SoundsPlayer.h"

std::shared_ptr<SoundsPlayer> SoundsPlayer::getInstance()
{
	static std::shared_ptr<SoundsPlayer> instance{ new SoundsPlayer };
	return instance;
}

void SoundsPlayer::play(const sf::SoundBuffer& buffer)
{
	bool soundPlayed = false;
	// get next available sound
	for (auto& sound : m_sounds)
	{
		if (sound.getStatus() != sf::SoundSource::Status::Playing)
		{
			sound.setBuffer(buffer);
			sound.play();
			soundPlayed = true;
			break;
		}
	}
	if (!soundPlayed) // no sounds available
	{
		sf::Sound sound;
		m_sounds.push_back(sound);
		size_t last = m_sounds.size() - 1;
		m_sounds[last].setBuffer(buffer);
		m_sounds[last].play();
	}
}
