#pragma once

class Render;
class MeshShader;
struct Camera;
struct SceneNode;

class Scene
{
public:
	Scene(Render* render);
	~Scene();
	void Init();
	Camera* GetCamera() { return camera; }
	void Add(SceneNode* node)
	{
		assert(node);
		nodes.push_back(node);
	}
	void Draw();

private:
	void BeginScene();
	void EndScene();

	Render* render;
	vector<SceneNode*> nodes;
	Camera* camera;
	MeshShader* shader;
};
