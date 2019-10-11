#include "Keyboard.h"


Keyboard::Keyboard(void)
{
	w = false;
	a = false;
	s = false;
	d = false;
	z = false;
	x = false;
	isSet = false;
	Keytimer = NULL;
	Keytimer = new Timer;
}


Keyboard::~Keyboard(void)
{
	delete Keytimer;
	Keytimer = NULL;
}

void Keyboard::SetKeyDown(char key)
{
	if(key == 'w')
	{
		Keytimer->StartTime();
		w = true;
	}
	if(key == 'a')
	{
		Keytimer->StartTime();
		a = true;
	}
	if(key == 's')
	{
		Keytimer->StartTime();
		s = true;
	}
	if(key == 'd')
	{
		Keytimer->StartTime();
		d = true;
	}
	if(key == 'z')
	{
		Keytimer->StartTime();
		z = true;
	}
	if(key == 'x')
	{
		Keytimer->StartTime();
		x = true;
	}
}

void Keyboard::SetKeyUp(char key)
{
	if(key == 'w')
	{
		KeyDownTime = Keytimer->GetTime();
		w = false;
	}
	if(key == 'a')
	{
		KeyDownTime = Keytimer->GetTime();
		a = false;
	}
	if(key == 's')
	{
		KeyDownTime = Keytimer->GetTime();
		s = false;
	}
	if(key == 'd')
	{
		KeyDownTime = Keytimer->GetTime();
		d = false;
	}
	if(key == 'z')
	{
		KeyDownTime = Keytimer->GetTime();
		z = false;
	}
	if(key == 'x')
	{
		KeyDownTime = Keytimer->GetTime();
		x = false;
	}
}

bool Keyboard::isKeyDown(char key)
{
	/*
	if(key == 'w')
	{
	return w;
	}
	if(key == 'a')
	{
	return a;
	}
	if(key == 's')
	{
	return s;
	}
	if(key == 'd')
	{
	return d;
	}
	if(key == 'z')
	{
	return z;
	}
	if(key == 'x')
	{
	return x;
	}
	*/
	switch(key)
	{
	case('w'):
		return w;
		break;
	case('a'):
		return a;
		break;
	case('s'):
		return s;
		break;
	case('d'):
		return d;
		break;
	case('z'):
		return z;
		break;
	case('x'):
		return x;
		break;
	default:
		return false;
		break;

	}
}

bool Keyboard::AsyncIsKeyDown(int key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void Keyboard::handleKeys()
{

};