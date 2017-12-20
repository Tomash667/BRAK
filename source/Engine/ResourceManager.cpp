#include "Pch.h"
#include "Core.h"
#include "ResourceManager.h"
#include "Render.h"
#include "Mesh.h"
#include "Texture.h"
#include "WICTextureLoader.h"
#include "QmshLoader.h"

ResourceManager::ResourceManager(Render* render) : render(render)
{
	assert(render);
}

ResourceManager::~ResourceManager()
{
	for(Resource* res : resources)
		delete res;
	delete qmsh_loader;
}

void ResourceManager::Init()
{
	qmsh_loader = new QmshLoader(this, render->GetDevice(), render->GetContext());
}

Mesh* ResourceManager::GetMesh(const string& id)
{
	static Resource res_search;
	res_search.name = id;
	auto it = resources.find(&res_search);
	if(it != resources.end())
		return checked_cast<Mesh*>(*it);

	try
	{
		cstring path = Format("data/%s", id.c_str());
		auto mesh = qmsh_loader->Load(path);

		mesh->name = id;
		mesh->type = Resource::Mesh;
		resources.insert(mesh);

		return mesh;
	}
	catch(cstring err)
	{
		throw Format("Failed to load mesh '%s': %s", id.c_str(), err);
	}
}

Texture* ResourceManager::GetTexture(const string& id)
{
	static Resource res_search;
	res_search.name = id;
	auto it = resources.find(&res_search);
	if(it != resources.end())
		return checked_cast<Texture*>(*it);

	cstring path = Format("data/%s", id.c_str());
	wchar_t path_w[MAX_PATH];
	size_t len;
	mbstowcs_s(&len, path_w, path, MAX_PATH);

	ID3D11ShaderResourceView* view;
	HRESULT result = CreateWICTextureFromFile(render->GetDevice(), render->GetContext(), path_w, nullptr, &view);
	if(FAILED(result))
		throw Format("Failed to load texture '%s' (%u).", id.c_str(), result);

	auto tex = new Texture;
	tex->name = id;
	tex->type = Resource::Texture;
	tex->tex = view;
	resources.insert(tex);

	return tex;
}
