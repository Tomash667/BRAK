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

Game::Game() : engine(nullptr)
{
}

Game::~Game()
{
	delete engine;
}

bool Game::Run()
{
	bool ok = false;
	if(Init())
	{
		ok = true;
		engine->Run();
	}
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
	engine = new Engine;

	Engine::StartupOptions options;
	options.title = "BRAK";
	options.handler = this;
	engine->Init(options);

	input = engine->GetInputManager();
	scene = engine->GetScene();
	camera = scene->GetCamera();
	res_mgr = engine->GetResourceManager();
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

void Game::OnUpdate(float dt)
{
	if(input->Down(Key::Escape) || (input->Down(Key::Alt) && input->Down(Key::F4)))
	{
		engine->Shutdown();
		return;
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
		case 1: // right
			dir = PI * 0 / 4;
			break;
		case -9: // right backward
			dir = PI * 1 / 4;
			break;
		case -10: // backward
			dir = PI * 2 / 4;
			break;
		case -11: // left backward
			dir = PI * 3 / 4;
			break;
		case -1: // left
			dir = PI * 4 / 4;
			break;
		case 9: // left forward
			dir = PI * 5 / 4;
			break;
		case 10: // forward
		//default:
			dir = PI * 6 / 4;
			break;
		case 11: // right forward
			dir = PI * 7 / 4;
			break;
		}

		// move in camera direction (add extra rotation because of model orientation)
		dir = Clip(dir + camera->rot.x + PI / 2);

		player->pos += Vec3(cos(dir) * 3 * dt, 0, sin(dir) * 3 * dt);
	}

	player->mesh_inst->Update(dt);
}
