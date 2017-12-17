#pragma once

#include "Engine.h"

class Game
{
public:
	bool Run();

private:
	bool Init();
	void Loop();
	void Shutdown();

	Window* window;
	Render* render;
	Scene* scene;
	ResourceManager* res_mgr;
	InputManager* input;
};
