#include "Timer.h"

void Timer::start()
{
	m_StartTime = std::chrono::system_clock::now();
	m_bRunning = true;
}

bool Timer::isStarted()
{
	return m_bRunning;
}

void Timer::stop()
{
	m_EndTime = std::chrono::system_clock::now();
	m_bRunning = false;
}

void Timer::restart()
{
	if (isStarted())
	{
		stop();
		start();
	}
}

double Timer::elapsedMilliseconds()
{
	std::chrono::time_point<std::chrono::system_clock> endTime;

	if (m_bRunning)
	{
		endTime = std::chrono::system_clock::now();
	}
	else
	{
		endTime = m_EndTime;
	}

	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

double Timer::elapsedSeconds()
{
	return elapsedMilliseconds() / 1000.0;
}