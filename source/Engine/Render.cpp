#include "Pch.h"
#include "Core.h"
#include "Render.h"
#include "Window.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>

struct Globals
{
	Matrix matWorldViewProj;
	Vec4 color;
};

Render::Render(Window* window) : window(window)
{

}

Render::~Render()
{
	m_rasterState->Release();
	m_layout->Release();
	m_vertexBuffer->Release();
	globals->Release();
	m_pixelShader->Release();
	m_vertexShader->Release();
	m_depthStencilView->Release();
	render_target->Release();
	swap_chain->Release();
	context->Release();


//#ifdef _DEBUG
//	ID3D11Debug* debug;
//	device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);
//	if(debug)
//	{
//		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//		debug->Release();
//	}
//#endif


	device->Release();
}

void Render::Init()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();

	context->OMSetRenderTargets(1, &render_target, m_depthStencilView);

	CompileShaders();
	CreateCbuffer();
	CreateModel();
	SetViewport();

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE; //D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	/*result =*/ device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	context->RSSetState(m_rasterState);
}

void Render::CreateDeviceAndSwapChain()
{
	HRESULT result;
	/*IDXGIFactory* factory;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	throw Format("Failed to create IDXGIFactory (%u).", result);

	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	throw Format("Failed to create IDXGIAdapter (%u).", result);*/


	DXGI_SWAP_CHAIN_DESC swap_desc = { 0 };
	swap_desc.BufferCount = 1;
	swap_desc.BufferDesc.Width = 1024;
	swap_desc.BufferDesc.Height = 768;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/*if(m_vsync_enabled)
	{
	swap_desc.BufferDesc.RefreshRate.Numerator = numerator;
	swap_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else*/
	{
		swap_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swap_desc.OutputWindow = (HWND)window->GetHandle();

	// Turn multisampling off.
	swap_desc.SampleDesc.Count = 1;
	swap_desc.SampleDesc.Quality = 0;

	swap_desc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	// ???
	swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	// ???
	swap_desc.Flags = 0;


	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	int flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, &feature_level, 1,
		D3D11_SDK_VERSION, &swap_desc, &swap_chain, &device, nullptr, &context);
	if(FAILED(result))
		throw Format("Failed to create device and swap chain (%u).", result);
}

void Render::CreateRenderTargetView()
{
	HRESULT result;
	ID3D11Texture2D* back_buffer;
	result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
	if(FAILED(result))
		throw Format("Failed to get back buffer (%u).", result);

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(back_buffer, NULL, &render_target);
	if(FAILED(result))
		throw Format("Failed to create render target view (%u).", result);

	// Release pointer to the back buffer as we no longer need it.
	back_buffer->Release();
}

void Render::CreateDepthStencilView()
{
	HRESULT result;

	// create depth buffer texture
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = 1024;
	depthBufferDesc.Height = 768;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	ID3D11Texture2D* m_depthStencilBuffer;
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
		throw Format("Failed to create depth buffer texture (%u).", result);

	//==================================================================
	// Depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ID3D11DepthStencilState* m_depthStencilState;
	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if(FAILED(result))
		throw Format("Failed to create depth stencil state (%u).", result);

	context->OMSetDepthStencilState(m_depthStencilState, 1);
	m_depthStencilState->Release();

	//==================================================================
	// Initailze the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc {};

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
		throw Format("Failed to create depth stencil view (%u).", result);

	m_depthStencilBuffer->Release();
}

void Render::CompileShaders()
{
	auto vertexShaderBuf = CompileShader("simple.hlsl", "vs_simple", true);
	auto pixelShaderBuf = CompileShader("simple.hlsl", "ps_simple", false);

	HRESULT result = device->CreateVertexShader(vertexShaderBuf->GetBufferPointer(), vertexShaderBuf->GetBufferSize(), nullptr, &m_vertexShader);
	if(FAILED(result))
		throw Format("Failed to create vertex shader (%u).", result);

	result = device->CreatePixelShader(pixelShaderBuf->GetBufferPointer(), pixelShaderBuf->GetBufferSize(), nullptr, &m_pixelShader);
	if(FAILED(result))
		throw Format("Failed to create pixel shader (%u).", result);


	D3D11_INPUT_ELEMENT_DESC desc[1];
	desc[0].SemanticName = "POSITION";
	desc[0].SemanticIndex = 0;
	desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc[0].InputSlot = 0;
	desc[0].AlignedByteOffset = 0;
	desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[0].InstanceDataStepRate = 0;

	result = device->CreateInputLayout(desc, 1, vertexShaderBuf->GetBufferPointer(), vertexShaderBuf->GetBufferSize(), &m_layout);
	if(FAILED(result))
		throw Format("Failed to create input layout (%u).", result);

	vertexShaderBuf->Release();
	pixelShaderBuf->Release();
}

ID3DBlob* Render::CompileShader(cstring filename, cstring function, bool vertex)
{
	assert(filename && function);

	uint flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	cstring path = Format("../shaders/%s", filename);
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	wchar_t path_w[MAX_PATH];
	size_t len;
	mbstowcs_s(&len, path_w, path, MAX_PATH);
	HRESULT result = D3DCompileFromFile(path_w, nullptr, nullptr, function, vertex ? "vs_5_0" : "ps_5_0", flags, 0, &shaderBlob, &errorBlob);
	if(FAILED(result))
	{
		if(shaderBlob)
			shaderBlob->Release();
		if(errorBlob)
		{
			auto err = (cstring)errorBlob->GetBufferPointer();
			auto msg = Format("Failed to compile '%s' function '%s': %s (code %u).", path, function, err, result);
			errorBlob->Release();
			throw msg;
		}
		else
			throw Format("Failed to compile '%s' function '%s' (code %u).", path, function, result);
	}

	if(errorBlob)
	{
		auto err = (cstring)errorBlob->GetBufferPointer();
		printf("Shader '%s' warnings: %s", path, err);
		errorBlob->Release();
	}

	return shaderBlob;
}

void Render::CreateCbuffer()
{
	D3D11_BUFFER_DESC desc;

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Globals);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&desc, NULL, &globals);
	if(FAILED(result))
		throw Format("Failed to create buffer (%u).", result);
}

struct Vertex
{
	Vec3 pos;
};

void Render::CreateModel()
{
	Vertex v[3] = {
		Vec3(-1.f, -1.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(1.f, -1.f, 0.f)
	};

	D3D11_BUFFER_DESC v_desc;
	v_desc.Usage = D3D11_USAGE_DEFAULT;
	v_desc.ByteWidth = sizeof(v);
	v_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	v_desc.CPUAccessFlags = 0;
	v_desc.MiscFlags = 0;
	v_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA v_data;
	v_data.pSysMem = v;
	v_data.SysMemPitch = 0;
	v_data.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&v_desc, &v_data, &m_vertexBuffer);
	if(FAILED(result))
		throw Format("Failed to create vertex buffer (%u).", result);
}

void Render::SetViewport()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = 1024.f;
	viewport.Height = 768.f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	context->RSSetViewports(1, &viewport);
}

RNG _RNG;

void Render::Draw()
{
	float color[4];

	static float r = 0.f;
	r += 0.001f;

	// Setup the color to clear the buffer to.
	color[0] = 0;
	color[1] = 0.5f;
	color[2] = 1;
	color[3] = 1;

	// Clear the back buffer.
	context->ClearRenderTargetView(render_target, color);
	context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);


	Matrix matWorld = Matrix::RotationY(r).Transpose(),
		matView = Matrix::CreatePerspectiveFieldOfView(PI / 4, 1024.f / 768.f, 0.1f, 100.f).Transpose(),
		matProj = Matrix::CreateLookAt(Vec3(0, 0, -3), Vec3(0, 0, 0), Vec3(0, 1, 0)).Transpose();
	// transpose ???


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(globals, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Globals& g = *(Globals*)mappedResource.pData;
	g.matWorldViewProj = matWorld * matView * matProj;
	g.color = Vec4(1, 0, 0, 1);
	context->Unmap(globals, 0);

	context->VSSetConstantBuffers(0, 1, &globals);
	context->PSSetConstantBuffers(0, 1, &globals);
	context->IASetInputLayout(m_layout);


	context->VSSetShader(m_vertexShader, nullptr, 0);
	context->PSSetShader(m_pixelShader, nullptr, 0);

	uint stride = sizeof(Vertex),
		offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->Draw(3, 0);


	bool m_vsync_enabled = false;

	swap_chain->Present(m_vsync_enabled ? 1 : 0, 0);
}
