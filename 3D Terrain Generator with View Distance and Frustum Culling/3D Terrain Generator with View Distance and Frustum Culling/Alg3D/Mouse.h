#pragma once
#include <Windows.h>
class Mouse
{
public:
	Mouse(void);
	~Mouse(void);
private:
	int mposx;
	int mposy;
	int deltaposx;
	int deltaposy;
	bool isMouseLook;
public:
	bool Mouse::GetMouseLook(void);
	void Mouse::SetMouseLook(bool mouselook);
	int Mouse::GetPosX(void);
	int Mouse::GetPosY(void);
	void Mouse::SetPosX(int mx);
	void Mouse::SetPosY(int my);

	void Mouse::SetPos(int mx, int my);

	int Mouse::GetDeltaPosX(void);
	int Mouse::GetDeltaPosY(void);
	void Mouse::SetDeltaPosX(int dx);
	void Mouse::SetDeltaPosY(int dy);
	void Mouse::CalcDelta(int newmx, int newmy);

	void Mouse::CapturePos(void);
};

