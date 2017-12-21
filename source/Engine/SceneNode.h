#pragma once

#include "MeshInstance.h"

struct SceneNode
{
	void SetMesh(Mesh* mesh)
	{
		this->mesh = mesh;
		this->mesh_inst = nullptr;
	}
	void SetMesh(MeshInstance* mesh_inst)
	{
		this->mesh = mesh_inst->mesh;
		this->mesh_inst = mesh_inst;
	}

	Mesh* mesh;
	MeshInstance* mesh_inst;
	Vec3 pos, rot;
};
