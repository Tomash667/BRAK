#pragma once

#include "Resource.h"

class Render;
struct Mesh;
struct QmshLoader;
struct Texture;

class ResourceManager
{
	struct ResourceComparer
	{
		bool operator () (const Resource* r1, const Resource* r2) const
		{
			return _stricmp(r1->name.c_str(), r2->name.c_str()) > 0;
		}
	};
	typedef std::set<Resource*, ResourceComparer> ResourceContainer;
	typedef ResourceContainer::iterator ResourceIterator;

public:
	ResourceManager(Render* render);
	~ResourceManager();
	void Init();
	Mesh* GetMesh(const string& name);
	Texture* GetTexture(const string& name);

private:
	Render* render;
	QmshLoader* qmsh_loader;
	ResourceContainer resources;
};
