#include "Pch.h"
#include "Core.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Render.h"
#include "Window.h"
#include "MeshShader.h"
#include <d3d11.h>

Scene::Scene(Render* render) : render(render)
{
	assert(render);

	camera = new Camera;
	camera->from = Vec3(5, 5, 5);
	camera->to = Vec3::Zero;

	shader = new MeshShader;
}

Scene::~Scene()
{
	delete camera;
	delete shader;
}

void Scene::Init()
{
	shader->Init(render);
}

void Scene::Draw()
{
	BeginScene();

	shader->SetParams();

	auto& wnd_size = render->GetWindow()->GetSize();

	Matrix matWorld,
		matProj = Matrix::CreatePerspectiveFieldOfView(PI / 4, float(wnd_size.x) / wnd_size.y, 0.1f, 100.f),
		matView = Matrix::CreateLookAt(camera->from, camera->to, Vec3(0, 1, 0)),
		matCombined;

	static float r = 0;
	r += 0.0001f;

	for(auto node : nodes)
	{
		matWorld = Matrix::RotationY(r);
		//matWorld = Matrix::Translation(node->pos);
		matCombined = matWorld * matView * matProj;
		shader->SetBuffer(matCombined);
		shader->Draw(node->mesh);
	}

	EndScene();
}

void Scene::BeginScene()
{
	auto context = render->GetContext();
	context->ClearRenderTargetView(render->GetRenderTarget(), Vec4(0, 0.5f, 1.f, 1.f));
	context->ClearDepthStencilView(render->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Scene::EndScene()
{
	bool m_vsync_enabled = false;
	render->GetSwapChain()->Present(m_vsync_enabled ? 1 : 0, 0);
}
