#pragma once
#include "GameObject.h"

class Background : public GameObject
{
public:
	Background() : GameObject(0, 0, 0, 0, false) {}
	Background(double x, double y, double w, double h, bool animating, const std::string& backgroundMusicFileName) : 
		GameObject(x, y, w, h, animating),
		m_backgroundMusicFileName(backgroundMusicFileName)
	{}

	Definitions::ObjectType getType() override;

	void playAudio() override;

	std::ostringstream getCurrentState() override;
private:
	std::string m_backgroundMusicFileName;
};