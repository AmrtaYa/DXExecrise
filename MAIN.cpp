#include "DirectXMath.h"
#include "iostream"
#include "windows.h"
#include "DirectXApp.h"
using namespace DirectX;
using namespace std;
//����
void ExecrisePage29();
ostream& XM_CALLCONV operator<<(ostream& os, XMVECTOR v);
ostream& XM_CALLCONV operator<<(ostream& os, XMMATRIX v);
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	return DirectXApp::GetDXApp(hInstance, nCmdShow)->Run();
}
void Start()
{
	//����������к������б�
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	//����������   ˫�ػ���Double buffer �� ������

	//������������

	//����Ŀ����Ⱦ��ͼ

	//������Ȼ��壬ģ�建���Լ���ͼ

	//�����ӿ�

	//���òü�����


}
ostream& XM_CALLCONV operator<<(ostream& os, XMMATRIX v)
{
	for (size_t i = 0; i < 4; i++)
	{
		cout << v.r[i];
	}
	return os;
}
ostream& XM_CALLCONV operator<<(ostream& os, XMVECTOR v)
{
	XMFLOAT4 float4;
	XMStoreFloat4(&float4, v);

	os << "(" << float4.x << "," << float4.y << "," << float4.z << "," << float4.w << ")" << endl;

	return os;

}
void ExecrisePage29()
{
	//1.
	//u(1.2) v (3,4)
	//a u+v
	XMVECTORF32 u = { 1,2 };
	XMVECTORF32 v = { 3,4 };
	cout << "u+v = " << (u.v + v.v) << endl;
	cout << "u-v = " << (u.v - v.v) << endl;
	cout << "2u+0.5v = " << (2 * u.v - 0.5 * v.v) << endl;
	cout << "-2u+v = " << (-2 * u.v + v.v) << endl;
	cout << "--------------------2.------------------------------------" << endl;
	u = { 3,-4,1 };
	cout << "--------------------11.------------------------------------" << endl;
	XMVECTORF32 n = { -2,1 };
	XMVECTORF32 g = { 0,-9.8 };
	XMVECTOR gParaller = (XMVector2Dot(g.v, n.v) / XMVector2LengthSq(n.v)) * n.v; //G����N��ͶӰ��ʽ
	XMVECTOR gVertical = g.v - gParaller;//ͶӰ������ˣ���ô������ͶӰ�ʹ�ֱ��ɵ��Ǿ���һ��ֱ��������
	cout << "G����N��ͶӰ = " << gParaller;
	cout << "G����N�Ĵ�ֱ = " << gVertical;

}
