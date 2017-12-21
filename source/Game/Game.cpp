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
#include "Timer.h"
#include "MeshInstance.h"

#include <conio.h>

bool Game::Run()
{
	bool ok = false;
	if(Init())
	{
		ok = true;
		Loop();
	}
	Shutdown();
	return ok;
}

bool Game::Init()
{
	try
	{
		input = new InputManager;

		window = new Window(input);
		window->SetTitle("BRAK");
		window->Init();

		render = new Render(window);
		render->Init();

		scene = new Scene(render);
		scene->Init();

		res_mgr = new ResourceManager(render);
		res_mgr->Init();

		auto mesh_inst = new MeshInstance(res_mgr->GetMesh("human.qmsh"));
		mesh_inst->Play("idzie", 0, 0);

		node = new SceneNode;
		node->SetMesh(mesh_inst);
		node->pos = Vec3(0, 0, 0);
		node->rot = Vec3(0, 0, 0);
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
	Timer timer;
	while(window->Tick())
	{
		float dt = timer.Tick();
		node->rot.y += dt;
		node->mesh_inst->Update(dt);
		if(input->Down(Key::Escape))
			break;
		scene->Draw();
		input->Update();
	}
}

void Game::Shutdown()
{
	delete res_mgr;
	delete scene;
	delete render;
	delete window;
	delete input;
}
