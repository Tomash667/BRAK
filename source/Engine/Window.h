#pragma once

class InputManager;

class Window
{
public:
	typedef void* Handle;

	Window(InputManager* input);
	void Init();
	bool Tick();
	Handle GetHandle() { return hwnd; }
	const Int2& GetSize() const { return size; }
	void SetTitle(cstring title);

private:
	void RegisterClass();
	void AdjustSize();
	void Create();
	void Center();
	IntPointer HandleEvents(uint msg, IntPointer wParam, UIntPointer lParam);

	InputManager* input;
	Handle hwnd;
	string title;
	Int2 size, real_size;
};
