#pragma once

struct QmshLoader
{
	QmshLoader(ResourceManager* res_mgr);
	Mesh* Load(cstring path);

	ResourceManager* res_mgr;
};
