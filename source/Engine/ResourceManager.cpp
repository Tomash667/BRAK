#include "Pch.h"
#include "Core.h"
#include "ResourceManager.h"
#include "Mesh.h"

Mesh* ResourceManager::GetMesh(const string& id)
{
	return new Mesh;
}
