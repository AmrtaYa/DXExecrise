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
	int Run();
	bool Initalize();
	virtual LRESULT AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	bool succeed;

	static DirectXApp* GetDXApp(HINSTANCE hInstance, int nCmdShow);
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
	ComPtr < ID3D12Fence> fence;
	//MSAA
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQuailtyLv;
	DXGI_FORMAT mBackBufferFormat;
	UINT m4xMsaaQuality;



	//函数
	bool D3DInit();
	bool WindowsInit();
	void FenceInit();
	void MSAACheckInit();

	//其他函数
	void CloseWindows();

};