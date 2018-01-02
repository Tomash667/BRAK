#pragma once

class InputManager;

class Window
{
public:
	typedef void* Handle;

	Window(InputManager* input);
	void Init();
	bool Tick();
	void UnlockCursor(bool lock_on_focus);
	void LockCursor();

	bool IsActive() const { return active; }
	bool IsCursorLocked() const { return locked_cursor; }
	bool IsCursorVisible() const { return cursor_visible; }

	Handle GetHandle() { return hwnd; }
	const Int2& GetSize() const { return size; }

	void SetTitle(cstring title);

private:
	void RegisterClass();
	void AdjustSize();
	void Create();
	void Center();
	IntPointer HandleEvents(uint msg, IntPointer wParam, UIntPointer lParam);
	IntPointer ProcessMouseButton(IntPointer wParam);
	void UpdateActivity(bool is_active);
	void PlaceCursor();

	InputManager* input;
	Handle hwnd;
	string title;
	Int2 size, real_size, unlock_point, activation_point;
	uint frames;
	float frame_time, fps;
	bool fullscreen, active, replace_cursor, locked_cursor, lock_on_focus, cursor_visible;
};
