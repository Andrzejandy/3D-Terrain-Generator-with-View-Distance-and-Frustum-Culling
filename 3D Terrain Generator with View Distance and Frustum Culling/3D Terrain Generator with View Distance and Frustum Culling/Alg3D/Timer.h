#pragma once
#include <Windows.h>
class Timer
{
public:
	Timer(void);
	~Timer(void);
private:
	DWORD startTime;
	double clocksPerSecond;
	__int64 TimerStart;

	int frameCount;
	int fps;

	__int64 TimeOld;
	double frameTime;
public:
	void Timer::StartTime(void);
	double Timer::GetFrameTime(void);
	double Timer::GetTime(void);

	void Timer::setFPS(int FPS);
	int Timer::getFPS(void);

	void Timer::setFPScount(int fpscount);
	int Timer::getFPScount(void);

};

