#include "Pch.h"
#include "Core.h"
#include "MeshInstance.h"

MeshInstance::MeshInstance(Mesh* mesh) : mesh(mesh), need_update(true)
{
	assert(mesh);
}

void MeshInstance::Update(float dt)
{

}

void MeshInstance::Play(cstring anim)
{

}

void MeshInstance::SetupBones()
{
	if(!need_update)
		return;
	need_update = false;
}
