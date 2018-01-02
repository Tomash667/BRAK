#pragma once

#include "Engine.h"

class Game
{
public:
	bool Run();

private:
	bool Init();
	void InitEngine();
	void InitGame();
	void Loop();
	bool UpdateGame(float dt);
	void Shutdown();

	Window* window;
	Render* render;
	Scene* scene;
	ResourceManager* res_mgr;
	InputManager* input;

	Camera* camera;
	SceneNode* player;
	bool moving = false;
};
