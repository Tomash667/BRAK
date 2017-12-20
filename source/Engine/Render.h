#pragma once

class Window;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D11RasterizerState;

class Render
{
public:
	Render(Window* window);
	~Render();
	void Init();
	Window* GetWindow() { return window; }
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetContext() { return context; }
	IDXGISwapChain* GetSwapChain() { return swap_chain; }
	ID3D11RenderTargetView* GetRenderTarget() { return render_target; }
	ID3D11DepthStencilView* GetDepthStencilView() { return depth_stencil_view; }
	ID3DBlob* CompileShader(cstring filename, cstring function, bool vertex);

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void SetViewport();
	void CreateRasterState();

	Window* window;
	IDXGISwapChain* swap_chain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* render_target;
	ID3D11DepthStencilView* depth_stencil_view;
	ID3D11RasterizerState* raster_state;
};
