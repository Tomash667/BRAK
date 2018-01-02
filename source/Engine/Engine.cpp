#include "Pch.h"
#include "Core.h"
#include "Engine.h"
#include "Timer.h"
#include "InputManager.h"
#include "Window.h"
#include "Render.h"
#include "Scene.h"
#include "ResourceManager.h"

Engine::Engine() : handler(nullptr), input(nullptr), window(nullptr), render(nullptr), scene(nullptr), res_mgr(nullptr), shutdown(false), frames(0),
frame_time(0), fps(0)
{
}

Engine::~Engine()
{
	delete scene;
	delete render;
	delete res_mgr;
	delete window;
	delete input;
}

void Engine::Init(const StartupOptions& options)
{
	assert(options.handler);

	handler = options.handler;

	input = new InputManager;

	window = new Window(input);
	window->SetTitle(options.title);
	window->SetSize(options.wnd_size);
	window->Init();

	render = new Render(window);
	render->Init();

	scene = new Scene(render);
	scene->Init();

	res_mgr = new ResourceManager(render);
	res_mgr->Init();
}

void Engine::Run()
{
	Timer timer;
	while(window->Tick())
	{
		float dt = timer.Tick();

		// calculate fps
		frames++;
		frame_time += dt;
		if(frame_time >= 1.f)
		{
			fps = frames / frame_time;
			frames = 0;
			frame_time = 0.f;
			printf("Fps: %g", fps);
		}

		// update game
		handler->OnUpdate(dt);
		if(shutdown)
			return;

		scene->Draw();
		input->Update();
	}
}
