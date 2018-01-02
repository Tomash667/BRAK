#pragma once

#include "EngineHandler.h"

class InputManager;
class Render;
class ResourceManager;
class Scene;
class Window;

class Engine
{
public:
	struct StartupOptions
	{
		string title;
		Int2 wnd_size;
		EngineHandler* handler;

		StartupOptions() : title("Window"), wnd_size(1024, 768), handler(nullptr)
		{
		}
	};

	Engine();
	~Engine();
	void Init(const StartupOptions& options);
	void Run();
	void Shutdown() { shutdown = true; }

	InputManager* GetInputManager() { return input; }
	Window* GetWindow() { return window; }
	Render* GetRender() { return render; }
	Scene* GetScene() { return scene; }
	ResourceManager* GetResourceManager() { return res_mgr; }

private:
	EngineHandler* handler;
	InputManager* input;
	Window* window;
	Render* render;
	Scene* scene;
	ResourceManager* res_mgr;
	uint frames;
	float frame_time, fps;
	bool shutdown;
};
