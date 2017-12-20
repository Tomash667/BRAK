#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

struct QmshLoader
{
	QmshLoader(ResourceManager* res_mgr, ID3D11Device* device, ID3D11DeviceContext* context);
	Mesh* Load(cstring path);

	ResourceManager* res_mgr;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	vector<byte> buf;
};
