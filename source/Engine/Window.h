#pragma once

class Window
{
public:
	typedef void* Handle;

	Window();
	void Init();
	bool Tick();
	Handle GetHandle() { return hwnd; }
	void SetTitle(cstring title);

private:
	void RegisterClass();
	void AdjustSize();
	void Create();
	void Center();
	IntPointer HandleEvents(uint msg, UIntPointer lParam, IntPointer wParam);

	Handle hwnd;
	string title;
	Int2 size, real_size;
};
