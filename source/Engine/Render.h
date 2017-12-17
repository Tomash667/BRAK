#pragma once

class Window;
struct IDXGIAdapter;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class Render
{
public:
	Render(Window* window);
	~Render();
	void Init();
	void Draw();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();

	Window* window;
	//IDXGIAdapter* adapter;
	IDXGISwapChain* swap_chain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* render_target;
	ID3D11DepthStencilView* m_depthStencilView;
};
