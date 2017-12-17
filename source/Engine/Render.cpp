#include "Pch.h"
#include "Core.h"
#include "Render.h"
#include "Window.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

Render::Render(Window* window) : window(window)
{

}

Render::~Render()
{
	m_depthStencilView->Release();
	render_target->Release();
	swap_chain->Release();
	context->Release();
	device->Release();
}

void Render::Init()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();

	context->OMSetRenderTargets(1, &render_target, m_depthStencilView);
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
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { 0 };

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

void Render::Draw()
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = 0;
	color[1] = 0.5f;
	color[2] = 1;
	color[3] = 1;

	// Clear the back buffer.
	context->ClearRenderTargetView(render_target, color);

	bool m_vsync_enabled = false;
	
	swap_chain->Present(m_vsync_enabled ? 1 : 0, 0);
}
