#pragma once
#include "Timer.h"
#include <iostream>
class Keyboard
{
public:
	Keyboard(void);
	~Keyboard(void);
private:
	bool w;
	bool a;
	bool s;
	bool d;
	bool z;
	bool x;
public:
	Timer* Keytimer;

	double KeyDownTime;
	bool isSet;

	void Keyboard::SetKeyDown(char key);
	void Keyboard::SetKeyUp(char key);
	bool Keyboard::isKeyDown(char key);
	void Keyboard::handleKeys();
	bool Keyboard::AsyncIsKeyDown(int key);

};

