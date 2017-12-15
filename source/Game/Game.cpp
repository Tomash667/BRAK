#include "Pch.h"
#include "Core.h"
#include "Game.h"
#include "Window.h"
#include "Render.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "SceneNode.h"
#include "Camera.h"

void Game::Run()
{
	Init();
}

void Game::Init()
{
	window = new Window;
	render = new Render(window);
	scene = new Scene(render);
	res_mgr = new ResourceManager;

	auto node = new SceneNode;
	node->mesh = res_mgr->GetMesh("cube.qmsh");
	node->pos = Vec3(0, 0, 0);
	scene->Add(node);

	auto camera = scene->GetCamera();
	camera->from = Vec3(-5, 5, -5);
	camera->to = Vec3(0, 0, 0);
}

void Game::Loop()
{
	while(window->Tick())
	{
		scene->Draw();
	}
}
