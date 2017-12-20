#include "Pch.h"
#include "Core.h"
#include "MeshShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Render.h"

MeshShader::MeshShader() : vertex_shader(nullptr), pixel_shader(nullptr), layout(nullptr), buffer(nullptr)
{
}

MeshShader::~MeshShader()
{
	if(vertex_shader)
		vertex_shader->Release();
	if(pixel_shader)
		pixel_shader->Release();
	if(layout)
		layout->Release();
	if(sampler)
		sampler->Release();
	if(buffer)
		buffer->Release();
}

void MeshShader::Init(Render* render)
{
	assert(render);
	this->render = render;

	auto device = render->GetDevice();

	// create vertex shader
	auto vs_buf = render->CompileShader("mesh.hlsl", "vs_mesh", true);
	HRESULT result = device->CreateVertexShader(vs_buf->GetBufferPointer(), vs_buf->GetBufferSize(), nullptr, &vertex_shader);
	if(FAILED(result))
		throw Format("Failed to create vertex shader (%u).", result);

	// create pixel shader
	auto ps_buf = render->CompileShader("mesh.hlsl", "ps_mesh", false);
	result = device->CreatePixelShader(ps_buf->GetBufferPointer(), ps_buf->GetBufferSize(), nullptr, &pixel_shader);
	if(FAILED(result))
		throw Format("Failed to create pixel shader (%u).", result);


	// create vertex layout
	D3D11_INPUT_ELEMENT_DESC desc[3];

	desc[0].SemanticName = "POSITION";
	desc[0].SemanticIndex = 0;
	desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[0].InputSlot = 0;
	desc[0].AlignedByteOffset = 0;
	desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[0].InstanceDataStepRate = 0;

	desc[1].SemanticName = "NORMAL";
	desc[1].SemanticIndex = 0;
	desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[1].InputSlot = 0;
	desc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[1].InstanceDataStepRate = 0;

	desc[2].SemanticName = "TEXCOORD";
	desc[2].SemanticIndex = 0;
	desc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	desc[2].InputSlot = 0;
	desc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	desc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[2].InstanceDataStepRate = 0;

	result = device->CreateInputLayout(desc, 3, vs_buf->GetBufferPointer(), vs_buf->GetBufferSize(), &layout);
	if(FAILED(result))
		throw Format("Failed to create input layout (%u).", result);

	vs_buf->Release();
	ps_buf->Release();

	// create cbuffer for shader
	D3D11_BUFFER_DESC cb_desc;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.ByteWidth = sizeof(Buffer);
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	result = device->CreateBuffer(&cb_desc, NULL, &buffer);
	if(FAILED(result))
		throw Format("Failed to create cbuffer (%u).", result);

	// create texture sampler
	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&sampler_desc, &sampler);
	if(FAILED(result))
		throw Format("Failed to create sampler state (%u).", result);
}

void MeshShader::SetParams()
{
	auto context = render->GetContext();

	context->IASetInputLayout(layout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetConstantBuffers(0, 1, &buffer);
	context->VSSetShader(vertex_shader, nullptr, 0);
	context->PSSetSamplers(0, 1, &sampler);
	context->PSSetShader(pixel_shader, nullptr, 0);
}

void MeshShader::SetBuffer(const Matrix& matWorldViewProj)
{
	auto context = render->GetContext();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Buffer& b = *(Buffer*)mappedResource.pData;
	b.matWorldViewProj = matWorldViewProj.Transpose();
	context->Unmap(buffer, 0);
}

void MeshShader::Draw(Mesh* mesh)
{
	auto context = render->GetContext();
	uint stride = sizeof(Vertex),
		offset = 0;
	context->IASetVertexBuffers(0, 1, &mesh->vb, &stride, &offset);
	context->IASetIndexBuffer(mesh->ib, DXGI_FORMAT_R16_UINT, 0);

	for(auto& sub : mesh->subs)
	{
		context->PSSetShaderResources(0, 1, sub.tex ? &sub.tex->tex : nullptr);
		context->DrawIndexed(sub.tris * 3, sub.min_ind, 0);
	}
}
