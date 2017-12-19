#include "Pch.h"
#include "Core.h"
#include "ResourceManager.h"
#include "QmshLoader.h"

QmshLoader::QmshLoader(ResourceManager* res_mgr) : res_mgr(res_mgr)
{
	assert(res_mgr);
}

Mesh* QmshLoader::Load(cstring path)
{
	FileReader f(path);
	if(!f)
		throw "Failed to open file.";

	Ptr<Mesh> mesh;

	// load header
	f(!f.Read(mesh->mesh->head))
		throw "Failed to read mesh header.";
	if(memcmp(mesh->head.format, "QMSH", 4) != 0)
		throw Format("Invalid file signature '%.4s'.", mesh->head.format);
	if(mesh->head.version < 12 || mesh->head.version > 20)
		throw Format("Invalid mesh version '%u'.", mesh->head.version);
	if(mesh->head.version < 20)
		throw Format("Unsupported mesh version '%u'.", mesh->head.version);
	if(mesh->head.n_bones >= 32)
		throw Format("Too many bones (%u).", mesh->head.n_bones);
	if(mesh->head.n_subs == 0)
		throw "Missing model mesh.";
	if(IS_SET(mesh->head.flags, F_ANIMATED) && !IS_SET(mesh->head.flags, F_STATIC))
	{
		if(mesh->head.n_bones == 0)
			throw "No bones.";
		if(mesh->head.n_groups == 0)
			throw "No bone groups.";
	}
	if(mesh->head.n_splits)
		throw "Mesh splits are unsupported.";

	return mesh.Pin();
}
