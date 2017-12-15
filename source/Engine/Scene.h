#pragma once

class Render;
struct Camera;
struct SceneNode;

class Scene
{
public:
	Scene(Render* render);
	Camera* GetCamera() { return camera; }
	void Add(SceneNode* node)
	{
		assert(node);
		nodes.push_back(node);
	}
	void Draw();

private:
	vector<SceneNode*> nodes;
	Camera* camera;
};
