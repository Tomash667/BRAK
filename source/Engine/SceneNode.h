#pragma once

struct Mesh;

struct SceneNode
{
	Mesh* mesh;
	Vec3 pos, rot;
};
