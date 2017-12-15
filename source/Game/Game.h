#pragma once

#include "Engine.h"

class Game
{
public:
	void Run();

private:
	void Init();
	void Loop();

	Window* window;
	Render* render;
	Scene* scene;
	ResourceManager* res_mgr;
};
