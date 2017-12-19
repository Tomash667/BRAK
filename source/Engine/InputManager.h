#pragma once

#include "Key.h"

class InputManager
{
	enum InputState
	{
		IS_UP,			// 00
		IS_RELEASED,	// 01
		IS_DOWN,		// 10
		IS_PRESSED		// 11
	};

public:
	bool Pressed(Key key) const { return keystate[(int)key] == IS_PRESSED; }
	bool Released(Key key) const { return keystate[(int)key] == IS_RELEASED; }
	bool Down(Key key) const { return keystate[(int)key] >= IS_DOWN; }
	bool Up(Key key) const { return keystate[(int)key] <= IS_RELEASED; }
	void Process(Key key, bool down);
	void Update();

private:
	vector<Key> to_release;
	byte keystate[256];
};
