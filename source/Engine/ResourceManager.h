#pragma once

struct Mesh;

class ResourceManager
{
public:
	Mesh* GetMesh(const string& name);
};
