#pragma once

#include <d3d11.h>

struct Mesh;
class Render;

class MeshShader
{
	struct Buffer
	{
		Matrix matWorldViewProj;
	};

public:
	MeshShader();
	~MeshShader();
	void Init(Render* render);
	void SetParams();
	void SetBuffer(const Matrix& matWorldViewProj);
	void Draw(Mesh* mesh);

private:
	Render* render;
	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* buffer;
	ID3D11SamplerState* sampler;
};
