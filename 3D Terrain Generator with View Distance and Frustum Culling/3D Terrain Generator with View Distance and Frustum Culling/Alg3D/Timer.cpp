#include "Timer.h"


Timer::Timer(void)
{
	//startTime =	timeGetTime(); 
	TimerStart = 0;
	clocksPerSecond = 0;
	TimeOld = 0;
}


Timer::~Timer(void)
{
}

void Timer::StartTime(void)
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	clocksPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	TimerStart = frequencyCount.QuadPart;
}

double Timer::GetTime(void)
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart-TimerStart)/clocksPerSecond;
}

double Timer::GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart-TimeOld;
	TimeOld = currentTime.QuadPart;

	if(tickCount < 0.0f)
		tickCount = 0.0f;
	return 	double(tickCount)/clocksPerSecond;
}

void Timer::setFPS(int FPS)
{
	fps = FPS;
}

int Timer::getFPS(void)
{
	return fps;
}

void Timer::setFPScount(int fpscount)
{
	frameCount = fpscount;
}
int Timer::getFPScount(void)
{
	return frameCount;
}