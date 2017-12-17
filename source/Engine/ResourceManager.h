#pragma once

struct Mesh;
struct Texture;

class ResourceManager
{
public:
	Mesh* GetMesh(const string& name);
	Texture* GetTexture(const string& name);
};
