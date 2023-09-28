#pragma once
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dUtil.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
using namespace Microsoft::WRL;
class DirectXApp
{
protected:

public:
	DirectXApp(HINSTANCE instance, int show);
	~DirectXApp();
	bool Initalize();
	virtual LRESULT AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static DirectXApp* GetDXApp(HINSTANCE hInstance, int nCmdShow);
	//外部常用方法
	int Run();

	//字段
	bool succeed;
	UINT mClientWidth;
	UINT mClientHeight;
	UINT m4xMsaaState;
	UINT SwapChainBufferCount;

private:
	//windows
	HWND ghMainWnd = 0;
	HINSTANCE hInstance;
	int nCmdShow;
	//DX
	//初始化
	ComPtr<ID3D12Debug> debug;
	ComPtr < IDXGIFactory4> factory;
	ComPtr < ID3D12Device> device;
	HRESULT result;
	ComPtr<IDXGIAdapter> apadater;
	//围栏
	ComPtr <ID3D12Fence> fence;
	//MSAA
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQuailtyLv;
	DXGI_FORMAT mBackBufferFormat;
	UINT m4xMsaaQuality;
	//创建命令队列和命令列表
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	//交换链
	DXGI_FORMAT mBackBufferFormat;
	ComPtr<IDXGISwapChain> mSwapChain;

	//函数
	//初始化函数
	bool D3DInit();
	bool WindowsInit();
	void FenceInit();
	void MSAACheckInit();
	void CreateCommandObjects();
	void CreateSwapChain();
	//其他函数
	void CloseWindows();

};