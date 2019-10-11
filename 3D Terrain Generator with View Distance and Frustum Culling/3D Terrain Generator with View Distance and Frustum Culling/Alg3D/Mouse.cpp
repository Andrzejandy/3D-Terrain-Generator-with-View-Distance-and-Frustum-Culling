#include "Mouse.h"

Mouse::Mouse()
{
	mposx = 0;
	mposy = 0;
	deltaposx = 0;
	deltaposy = 0;
	isMouseLook = false;
}

Mouse::~Mouse()
{

}

bool Mouse::GetMouseLook(void)
{
	return isMouseLook;
}

int Mouse::GetPosX(void)
{
	return mposx;
}
int Mouse::GetPosY(void)
{
	return mposy;
}
void Mouse::SetPosX(int mx)
{
	mposx = mx;
}
void Mouse::SetPosY(int my)
{
	mposy = my;
}

void Mouse::SetPos(int mx, int my)
{
	mposx = mx;
	mposy = my;
}

void Mouse::CapturePos(void)
{
	POINT mousepoint;
	GetCursorPos(&mousepoint);
	mposx = mousepoint.x;
	mposy = mousepoint.y;
}

int Mouse::GetDeltaPosX(void)
{
	return deltaposx;
}
int Mouse::GetDeltaPosY(void)
{
	return deltaposy;
}
void Mouse::SetDeltaPosX(int dx)
{
	deltaposx = dx;
}
void Mouse::SetDeltaPosY(int dy)
{
	deltaposy = dy;
}

void Mouse::CalcDelta(int newmx, int newmy)
{
	deltaposx = newmx - mposx;
	deltaposy = newmy - mposy;
	//	+			+
	if(mposx > 0 && mposy > 0)
	{

	}
	//	+			-
	if(mposx > 0 && mposy < 0)
	{

	}
	//	-			+
	if(mposx < 0 && mposy > 0)
	{

	}
	//	-			-
	if(mposx < 0 && mposy < 0)
	{

	}
}

void Mouse::SetMouseLook(bool mouselook)
{
	isMouseLook = mouselook;
}