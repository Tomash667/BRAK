#include "Pch.h"
#include "Core.h"
#include "ResourceManager.h"
#include "QmshLoader.h"
#include "Mesh.h"
#include <d3d11.h>

QmshLoader::QmshLoader(ResourceManager* res_mgr, ID3D11Device* device, ID3D11DeviceContext* context) : res_mgr(res_mgr), device(device), context(context)
{
	assert(res_mgr);
	assert(device);
	assert(context);
}

Mesh* QmshLoader::Load(cstring path)
{
	FileReader f(path);
	if(!f)
		throw "Failed to open file.";

	Ptr<Mesh> mesh;

	// load header
	if(!f.Read(mesh->head))
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
	if(IS_SET(mesh->head.flags, Mesh::F_ANIMATED) && !IS_SET(mesh->head.flags, Mesh::F_STATIC))
	{
		if(mesh->head.n_bones == 0)
			throw "No bones.";
		if(mesh->head.n_groups == 0)
			throw "No bone groups.";
	}
	if(IS_SET(mesh->head.flags, Mesh::F_PHYSICS | Mesh::F_ANIMATED | Mesh::F_TANGENTS || Mesh::F_SPLIT) || mesh->head.n_points > 0)
		throw Format("Not implemented mesh flags used (%u).", mesh->head.flags);

	// vertex buffer
	uint size = sizeof(Vertex) * mesh->head.n_verts;
	if(!f.Ensure(size))
		throw "Failed to read vertex data.";

	buf.resize(size);
	f.Read(buf.data(), size);
	Vertex* vd = (Vertex*)buf.data();

	D3D11_BUFFER_DESC v_desc;
	v_desc.Usage = D3D11_USAGE_DEFAULT;
	v_desc.ByteWidth = size;
	v_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	v_desc.CPUAccessFlags = 0;
	v_desc.MiscFlags = 0;
	v_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA v_data;
	v_data.pSysMem = buf.data();

	HRESULT result = device->CreateBuffer(&v_desc, &v_data, &mesh->vb);
	if(FAILED(result))
		throw Format("Failed to create vertex buffer (%u).", result);
	
	// index buffer
	size = sizeof(word) * mesh->head.n_tris * 3;
	if(!f.Ensure(size))
		throw "Failed to read index data.";

	buf.resize(size);
	f.Read(buf.data(), size);
	word* id = (word*)buf.data();

	v_desc.ByteWidth = size;
	v_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	v_data.pSysMem = buf.data();

	result = device->CreateBuffer(&v_desc, &v_data, &mesh->ib);
	if(FAILED(result))
		throw Format("Failed to create index buffer (%u).", result);

	// submeshes
	string filename;
	if(!f.Ensure(mesh->head.n_subs, Mesh::Submesh::MIN_SIZE))
		throw "Failed to read submesh data.";
	mesh->subs.resize(mesh->head.n_subs);
	uint index = 0;
	for(auto& sub : mesh->subs)
	{
		f >> sub.first;
		f >> sub.tris;
		f >> sub.min_ind;
		f >> sub.n_ind;
		f >> sub.name;
		f >> filename;

		if(!filename.empty())
			sub.tex = res_mgr->GetTexture(filename);
		else
			sub.tex = nullptr;

		// specular value
		f >> sub.specular_color;
		f >> sub.specular_intensity;
		f >> sub.specular_hardness;

		// normalmap
		if(IS_SET(mesh->head.flags, Mesh::F_TANGENTS))
		{
			f >> filename;
			if(!filename.empty())
			{
				sub.tex_normal = res_mgr->GetTexture(filename);
				f >> sub.normal_factor;
			}
			else
				sub.tex_normal = nullptr;
		}
		else
			sub.tex_normal = nullptr;

		// specular map
		f >> filename;
		if(!filename.empty())
		{
			sub.tex_specular = res_mgr->GetTexture(filename);
			f >> sub.specular_factor;
			f >> sub.specular_color_factor;
		}
		else
			sub.tex_specular = nullptr;

		if(!f)
			throw Format("Failed to read submesh %u.", index);

		++index;
	}

	return mesh.Pin();
}
