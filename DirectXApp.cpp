#include "DirectXApp.h"

DirectXApp* DXApp = nullptr;

DirectXApp* DirectXApp::GetDXApp(HINSTANCE hInstance, int nCmdShow)
{
	if (DXApp == nullptr)
	{
		DirectXApp* app = new DirectXApp(hInstance, nCmdShow);
		DXApp = app;
	}
	return DXApp;
}


LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto instance = DXApp;
	if (DXApp == nullptr)return DefWindowProc(hwnd, msg, wParam, lParam);
	return instance ->AppWndProc(hwnd, msg, wParam, lParam);
}
LRESULT DirectXApp::AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		CloseWindows();
		return 0;
	case WM_CREATE:
		//MessageBox(0, L"加载完毕", L"加载窗口", MB_OK);
		return 0;
	case WM_SIZE:
		return 0;
	case WM_ACTIVATE:
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
DirectXApp::DirectXApp(HINSTANCE instance, int show) : hInstance(instance), nCmdShow(show)
{
	succeed = Initalize();
}

DirectXApp::~DirectXApp()
{
}

bool DirectXApp::Initalize()
{
	if (!WindowsInit())
		return false;
	if (!D3DInit())
		return false;
	FenceInit();
	MSAACheckInit();
	return false;
}

bool DirectXApp::D3DInit()
{
	//初始化
#if defined(DEBUG)|| defined(_DEBUG)
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
#endif // defined(DEBUG)|| defined(_DEBUG)
	ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&factory)));
	result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
	if (FAILED(result))
	{
		ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&apadater)));
		ThrowIfFailed(D3D12CreateDevice(apadater.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
		return false;
	}
	return true;
}

bool DirectXApp::WindowsInit()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BasicWndClass";
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}
	ghMainWnd = CreateWindow(
		L"BasicWndClass",
		L"Win32Basic",
		WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0
	);
	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindows Failed", 0, 0);
		return false;
	}
	ShowWindow(ghMainWnd, nCmdShow);
	UpdateWindow(ghMainWnd);
	return true;
}

void DirectXApp::FenceInit()
{
	//创建围栏

	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	int mRtvDesriptSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);//Render Target View
	int mDsvDesriptSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);//Depth Stencil View 
	int mUavDesriptSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);//Unordered Access View 无序访问视图 

}
void DirectXApp::MSAACheckInit()
{
	//检查MSAA抗锯齿支持性
	msQuailtyLv.Format = mBackBufferFormat;
	msQuailtyLv.SampleCount = 4;
	msQuailtyLv.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQuailtyLv.NumQualityLevels = 0;
	ThrowIfFailed(device->CheckFeatureSupport
	(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQuailtyLv,
		sizeof(msQuailtyLv)
	));
	m4xMsaaQuality = msQuailtyLv.NumQualityLevels;
//	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");  好像现在还不支持
}

int DirectXApp::Run()
{
	MSG msg = { 0 };
	BOOL bRet = 1;
	while ((bRet = (GetMessage(&msg, 0, 0, 0)) != 0))
	{
		if (bRet == -1) {
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

void DirectXApp::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queue = {};
	queue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(device->CreateCommandQueue(&queue,IID_PPV_ARGS(&mCommandQueue)));
	ThrowIfFailed(device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())));
	ThrowIfFailed(device->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(mCommandList.GetAddressOf())));
	mCommandList->Close();
}

void DirectXApp::CreateSwapChain()
{
	mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	mSwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = ghMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ThrowIfFailed(factory->CreateSwapChain(
		mCommandQueue.Get(),
		&sd,
		mSwapChain.GetAddressOf()));
}

void DirectXApp::CloseWindows()
{
	int index = MessageBox(0, L"是否退出程序", L"退出窗口", MB_YESNO);
	if (index == IDYES)
		PostQuitMessage(0);
}
