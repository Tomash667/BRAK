#include "Pch.h"
#include "Core.h"
#include "Game.h"
#include "Window.h"
#include "Render.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "SceneNode.h"
#include "Camera.h"
#include "InputManager.h"

#include <conio.h>

bool Game::Run()
{
	if(!Init())
		return false;
	Loop();
	Shutdown();
	return true;
}

bool Game::Init()
{
	try
	{
		window = new Window;
		window->SetTitle("BRAK");
		window->Init();
		render = new Render(window);
		render->Init();
		scene = new Scene(render);
		res_mgr = new ResourceManager;
		input = new InputManager;

		auto node = new SceneNode;
		node->mesh = res_mgr->GetMesh("cube.qmsh");
		node->pos = Vec3(0, 0, 0);
		scene->Add(node);

		auto camera = scene->GetCamera();
		camera->from = Vec3(-5, 5, -5);
		camera->to = Vec3(0, 0, 0);

		return true;
	}
	catch(cstring err)
	{
		printf("FATAL ERROR:\n");
		printf(err);
		_getch();
		return false;
	}
}

void Game::Loop()
{
	while(window->Tick())
	{
		if(input->Down(Key::Escape))
			break;
		render->Draw();
	}
}

void Game::Shutdown()
{
	delete render;
}
