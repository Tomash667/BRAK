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
		InitEngine();
		InitGame();



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

void Game::InitEngine()
{
	input = new InputManager;

	window = new Window(input);
	window->SetTitle("BRAK");
	window->Init();

	render = new Render(window);
	render->Init();

	scene = new Scene(render);
	scene->Init();
	camera = scene->GetCamera();

	res_mgr = new ResourceManager(render);
	res_mgr->Init();
}

void Game::InitGame()
{
	auto floor = new SceneNode;
	floor->SetMesh(res_mgr->GetMesh("floor.qmsh"));
	floor->pos = Vec3(0, 0, 0);
	floor->rot = Vec3(0, 0, 0);
	scene->Add(floor);

	auto mesh_inst = new MeshInstance(res_mgr->GetMesh("human.qmsh"));
	mesh_inst->Play("stoi", 0, 0);

	player = new SceneNode;
	player->SetMesh(mesh_inst);
	//player->SetMesh(res_mgr->GetMesh("human.qmsh"));
	player->pos = Vec3(0, 0, 0);
	player->rot = Vec3(0, -PI / 2, 0);
	scene->Add(player);
	moving = false;

	camera->target = player;
	camera->h = 1.5f;
	camera->rot = Vec2(-PI / 2, 4.2875104f);
	camera->vrot = camera->rot;

	auto marker = new SceneNode;
	marker->SetMesh(res_mgr->GetMesh("marker.qmsh"));
	marker->pos = Vec3(0, 0, 0);
	marker->rot = Vec3(0, 0, 0);
	scene->Add(marker);
}

void Game::Loop()
{
	Timer timer;
	while(window->Tick())
	{
		float dt = timer.Tick();

		if(!UpdateGame(dt))
			break;

		scene->Draw();
		input->Update();
	}
}

bool Game::UpdateGame(float dt)
{
	if(input->Down(Key::Escape) || (input->Down(Key::Alt) && input->Down(Key::F4)))
		return false;

	int move = 0;
	if(input->Down(Key::W))
		move += 10;
	if(input->Down(Key::S))
		move -= 10;
	if(input->Down(Key::A))
		move -= 1;
	if(input->Down(Key::D))
		move += 1;

	if(move == 0)
	{
		if(moving)
		{
			moving = false;
			player->mesh_inst->Play("stoi", 0, 0);
		}
	}
	else
	{
		if(!moving)
		{
			moving = true;
			player->mesh_inst->Play("biegnie", 0, 0);
		}

		float dir;
		switch(move)
		{
		default:
		case 10:
			dir = PI;
			break;
		}
	}

	const float c_cam_angle_min = PI + 0.1f;
	const float c_cam_angle_max = PI * 1.8f - 0.1f;

	auto& mouse_dif = input->GetMouseMove();
	//mouse_dif.x = -1;
	camera->rot.x = Clip(camera->rot.x + float(mouse_dif.x) / 800);
	//camera->rot.y -= float(mouse_dif.y) / 400;
	if(camera->rot.y > c_cam_angle_max)
		camera->rot.y = c_cam_angle_max;
	if(camera->rot.y < c_cam_angle_min)
		camera->rot.y = c_cam_angle_min;
	camera->Update(dt);

	player->mesh_inst->Update(dt);

	return true;
}

void Game::Shutdown()
{
	delete res_mgr;
	delete scene;
	delete render;
	delete window;
	delete input;
}
