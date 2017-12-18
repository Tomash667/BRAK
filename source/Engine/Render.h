#pragma once

class Window;
struct IDXGIAdapter;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11RasterizerState;

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
	void CompileShaders();
	ID3DBlob* CompileShader(cstring filename, cstring function, bool vertex);
	void CreateCbuffer();
	void CreateModel();
	void SetViewport();

	Window* window;
	//IDXGIAdapter* adapter;
	IDXGISwapChain* swap_chain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* render_target;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* globals;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11RasterizerState* m_rasterState;
};
