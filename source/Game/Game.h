#pragma once

#include "Engine.h"
#include "EngineHandler.h"

struct Camera;
struct SceneNode;

class Game : public EngineHandler
{
public:
	Game();
	~Game();
	bool Run();

private:
	bool Init();
	void InitEngine();
	void InitGame();
	void OnUpdate(float dt) override;

	Engine* engine;
	Scene* scene;
	ResourceManager* res_mgr;
	InputManager* input;

	Camera* camera;
	SceneNode* player;
	bool moving = false;
};
